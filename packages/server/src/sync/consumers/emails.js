const _ = require('lodash');
const moment = require('moment');

const { Consumer } = require('./consumer');

const events = require('../events');
const emitter = require('../emitter');

const { Pagination } = require('../../data/classes/pagination');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');
const { Params } = require('../../data/classes/params');

const { assert, errs } = require('./errors');

/**
 * @todo: Rename into "EmailsConsumer".
 */
class Emails extends Consumer {
  constructor() {
    super();
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    let transaction;
    try {
      transaction = await this.notifications.getTransaction();
    } catch (e) {
      console.log('Emails Consumer Transaction Error', e.message, e);
      throw e;
    }

    try {
      const statuses = await this._getNotificationsStatusesToSend(transaction);

      for (let i = 0; i < statuses.length; i++) {
        const status = statuses[i];

        try {
          const notification = await this._getNotification(status);

          const agent = await this._getAgent(status);
          const preferences = await this._getPreferences(status);
          const user = await this._getUser(agent);

          await this._test(preferences, user);

          const result = await this._emitEmail(notification, user, agent);

          if (result === true) {
            await this._markNotificationAsSent(status, transaction);
          } else {
            throw new errs.ExternalError('Email is not sent');
          }

          console.log(
            `\x1b[35mNotification ID = "${_.get(notification, 'id')}", `
            + `Transaction ID = "${_.get(transaction, 'id')}", `
            + `Agent Preferences ID = "${_.get(preferences, 'id')}", `
            + `Agent ID = "${_.get(agent, 'id')}", `
            + `Result = "${result}".\x1b[0m`,
          );
        } catch (err) {
          console.log('Emails Consumer, pass sending due to', err.message);
          await this._notificationError(status, err, transaction);
        }
      }

      await this.notifications.commit(transaction);
    } catch (e) {
      console.log('Emails Consumer', e.message, e);
      await this.notifications.rollback(transaction);
    }

    return true;
  }

  /**
   * @param {object} transaction
   * @return {Promise<*>}
   * @protected
   */
  async _getNotificationsStatusesToSend(transaction) {
    const filter = new Filter({ sentAt: null, attemptCounterMax: 3 });
    const pagination = new Pagination({ limit: 10 });
    const options = new Options({ transaction, lock: true, skipLocked: true });
    return this.notificationStatuses.getAll(filter, pagination, options);
  }

  /**
   * @param {object} notificationStatus
   * @return {Promise<void>}
   * @protected
   */
  async _getNotification(notificationStatus) {
    const params = new Params({ id: notificationStatus.notificationId });
    const notification = await this.notifications.getOne(params);
    assert(!!notification, 'Notification is required');
    return notification;
  }

  /**
   * @param {object} notificationStatus
   * @return {Promise<void>}
   * @protected
   */
  async _getAgent(notificationStatus) {
    const params = new Params({ id: notificationStatus.agentId });
    return await this.agents.getOne(params);
  }

  /**
   * @param {object} notificationStatus
   * @param {object} transaction
   * @return {Promise<*>}
   * @protected
   */
  async _markNotificationAsSent(notificationStatus, transaction) {
    const params = new Params({ id: notificationStatus.id });
    const data = { sentAt: moment.utc().toDate() };
    const options = new Options({ transaction });
    return this.notificationStatuses.updateOne(params, data, options);
  }

  /**
   * @param {object} notificationStatus
   * @param {CustomError} err
   * @param {object} transaction
   * @return {Promise<*>}
   * @protected
   */
  async _notificationError(notificationStatus, err, transaction) {
    const weightError = err.weight || 1;
    const attemptCounterPrev = _.get(notificationStatus, 'attemptCounter') || 0;
    const attemptCounter = attemptCounterPrev + weightError;
    const errorAt = moment.utc().toDate();
    const errorReason = err.type;

    const params = new Params({ id: notificationStatus.id });
    const data = { errorAt, attemptCounter, errorReason };
    const options = new Options({ transaction });

    return this.notificationStatuses.updateOne(params, data, options);
  }

  /**
   * @param {object} notificationStatus
   * @return {Promise<*>}
   * @protected
   */
  async _getPreferences(notificationStatus) {
    const { agentId } = notificationStatus;
    return await this.preferences.getOneByAgentId(agentId);
  }

  /**
   * @param {object} agent
   * @return {Promise<void>}
   * @protected
   */
  async _getUser(agent) {
    const params = new Params({ agentId: agent.id });
    return await this.users.getOne(params);
  }

  /**
   * @param {object} agentPreferences
   * @param {object} user
   * @return {Promise<boolean>}
   * @protected
   */
  async _test(agentPreferences, user) {
    assert(user && !!_.get(user, 'email'),
      'Email is required.', errs.EmailAddressError);
    assert(agentPreferences && !agentPreferences.stopAllEmails,
      'Emails are turned off.', errs.PreferencesError);
    return true;
  }

  /**
   * @param {object} notification
   * @param {object} user
   * @param {object} agent
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
      console.log('Emails Consumer Emit Email', e.message, e);
      return false;
    }

    return true;
  }

  /**
   * @param {object} agent
   * @return {Promise<string | null>}
   * @protected
   */
  async _getAuthToken(agent) {
    if (!agent) return null;
    const agentId = agent.id;
    const token = await this.tokens.createAuthToken(agentId);
    assert(!!token, 'Token is required');
    assert(!!token.token, 'Token is required #2');
    return token.token;
  }
}

module.exports = {
  Emails,
};
