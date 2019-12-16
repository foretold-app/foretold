const _ = require('lodash');
const moment = require('moment');

const { Consumer } = require('./consumer');

const events = require('../events');
const emitter = require('../emitter');

const { Pagination } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');
const logger = require('../../lib/log');

const { assert, errs } = require('./errors');

const log = logger.module('sync/consumers/emails');

/**
 * @todo: Rename into "EmailsConsumer".
 */
class Emails extends Consumer {
  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    let transaction;
    try {
      transaction = await this.notifications.getTransaction();
    } catch (e) {
      log.trace('Emails Consumer Transaction Error', e.message, e);
      throw e;
    }

    try {
      const statuses = await this._getNotificationsStatusesToSend(transaction);

      for (let i = 0; i < statuses.length; i++) {
        const status = statuses[i];

        try {
          const agent = await this._getAgent(status);
          const user = await this._getUser(agent);
          const preferences = await this._getPreferences(status);
          await this._test(preferences, user);

          const notification = await this._getNotification(status);
          const result = await this._emitEmail(notification, user, agent);

          if (result === true) {
            await this._markNotificationAsSent(status, transaction);
          } else {
            throw new errs.ExternalError('Email is not sent');
          }

          log.trace(
            `\x1b[35mNotification ID = "${_.get(notification, 'id')}", `
            + `Transaction ID = "${_.get(transaction, 'id')}", `
            + `Agent Preferences ID = "${_.get(preferences, 'id')}", `
            + `Agent ID = "${_.get(agent, 'id')}", `
            + `Result = "${result}".\x1b[0m`,
          );
        } catch (err) {
          log.trace('Emails Consumer, pass sending due to', err.message);
          await this._notificationError(status, err, transaction);
        }
      }

      await this.notifications.commit(transaction);
    } catch (e) {
      log.trace('Emails Consumer', e.message, e);
      await this.notifications.rollback(transaction);
    }

    return true;
  }

  /**
   * @param {object} transaction
   * @return {Promise<Models.NotificationStatus[]>}
   * @protected
   */
  async _getNotificationsStatusesToSend(transaction) {
    const filter = new Filter({ sentAt: null, attemptCounterMax: 3 });
    const pagination = new Pagination({ limit: 10 });
    const options = new Options({ transaction, lock: true, skipLocked: true });
    return this.notificationStatuses.getAll(filter, pagination, options);
  }

  /**
   * @param {Models.NotificationStatus} notificationStatus
   * @return {Promise<Models.Notification>}
   * @protected
   */
  async _getNotification(notificationStatus) {
    const params = new Params({ id: notificationStatus.notificationId });
    const notification = await this.notifications.getOne(params);
    assert(!!notification, 'Notification is required');
    return notification;
  }

  /**
   * @param {Models.NotificationStatus} notificationStatus
   * @return {Promise<Models.Agent>}
   * @protected
   */
  async _getAgent(notificationStatus) {
    if (!notificationStatus.agentId) return null;

    const params = new Params({ id: notificationStatus.agentId });
    const agent = await this.agents.getOne(params);
    assert(!!agent, 'Agent is required');
    return agent;
  }

  /**
   * @param {Models.NotificationStatus} notificationStatus
   * @param {object} transaction
   * @return {Promise<Models.NotificationStatus>}
   * @protected
   */
  async _markNotificationAsSent(notificationStatus, transaction) {
    const params = new Params({ id: notificationStatus.id });
    const data = { sentAt: moment.utc().toDate() };
    const options = new Options({ transaction });
    return this.notificationStatuses.updateOne(params, data, options);
  }

  /**
   * @param {Models.NotificationStatus} notificationStatus
   * @param {CustomError} err
   * @param {object} transaction
   * @return {Promise<Models.NotificationStatus>}
   * @protected
   */
  async _notificationError(notificationStatus, err, transaction) {
    const weightError = err.weight || 1;
    const attemptCounterPrev = _.get(notificationStatus, 'attemptCounter') || 0;
    const attemptCounter = attemptCounterPrev + weightError;
    const errorAt = moment.utc().toDate();
    const errorReason = err.type || (new errs.InternalError()).type;

    const params = new Params({ id: notificationStatus.id });
    const data = { errorAt, attemptCounter, errorReason };
    const options = new Options({ transaction });

    return this.notificationStatuses.updateOne(params, data, options);
  }

  /**
   * @param {Models.NotificationStatus} notificationStatus
   * @return {Promise<Models.Preference | null>}
   * @protected
   */
  async _getPreferences(notificationStatus) {
    if (!notificationStatus.agentId) return null;

    const { agentId } = notificationStatus;
    const preferences = await this.preferences.getOneByAgentId(agentId);
    assert(!!preferences, 'Preferences is required');
    return preferences;
  }

  /**
   * @param {Models.Agent} agent
   * @return {Promise<Models.User | null>}
   * @protected
   */
  async _getUser(agent) {
    if (!agent) return null;

    const params = new Params({ agentId: agent.id });
    const user = await this.users.getOne(params);
    assert(!!user, `User is required for an agent "${agent.id}".`);
    return user;
  }

  /**
   * @param {Models.Preference} agentPreferences
   * @param {Models.User} user
   * @return {Promise<boolean>}
   * @protected
   */
  async _test(agentPreferences, user) {
    if (user) {
      assert(!!_.get(user, 'email'),
        'Email is required.', errs.EmailAddressError);
    }
    if (agentPreferences) {
      assert(!agentPreferences.stopAllEmails,
        'Emails are turned off.', errs.PreferencesError);
    }
    return true;
  }

  /**
   * @param {Models.Notification} notification
   * @param {Models.User} user
   * @param {Models.Agent} agent
   * @return {Promise<boolean>}
   * @protected
   */
  async _emitEmail(notification, user, agent) {
    const envelopeReplacements = notification.envelope.replacements || {};

    const token = await this._getAuthToken(agent);
    const replacements = { ...envelopeReplacements, token };

    const envelope = {
      replacements,
      authToken: token,
      to: notification.envelope.to || _.get(user, 'email'),
      body: notification.envelope.body || '',
      subject: notification.envelope.subject || '',
    };

    try {
      emitter.emit(events.MAIL, envelope);
    } catch (e) {
      log.trace('Emails Consumer Emit Email', e.message, e);
      return false;
    }

    return true;
  }

  /**
   * @param {Models.Agent} agent
   * @return {Promise<string | null>}
   * @protected
   */
  async _getAuthToken(agent) {
    if (!agent) return null;

    const token = await this.tokens.createAuthToken(agent.id);
    assert(!!token, 'Token is required');
    assert(!!token.token, 'Token is required #2');
    return token.token;
  }
}

module.exports = {
  Emails,
};
