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
   * @return {Promise<boolean>}
   */
  async main() {
    let transaction;
    try {
      transaction = await this.notifications.getTransaction();
    } catch (e) {
      console.log(`Emails Consumer Transaction Error`, e.message, e);
      throw e;
    }

    try {
      const agentNotifications = await this._getAgentsNotifications(transaction);

      for (let i = 0; i < agentNotifications.length; i++) {
        const agentNotification = agentNotifications[i];

        try {
          const notification = await this._getNotification(agentNotification);
          const agent = await this._getAgent(agentNotification);
          const agentPreferences = await this._getPreferences(agentNotification);
          const user = await this._getUser(agent);
          const _test = await this._test(agentPreferences, user);
          const result = await this._emitEmail(notification, user, agent);

          if (result === true) {
            await this._markNotificationAsSent(agentNotification, transaction);
          } else {
            throw new errs.ExternalError('Email is not sent');
          }

          console.log(
            `\x1b[35mNotification ID = "${notification.id}", ` +
            `Transaction ID = "${transaction.id}", ` +
            `Agent Preferences ID = "${agentPreferences.id}", ` +
            `Agent ID = "${agent.id}", ` +
            `Result = "${result}".\x1b[0m`
          );
        } catch (err) {
          console.log(`Emails Consumer, pass sending due to`, err.message);
          await this._notificationError(agentNotification, err, transaction);
        }
      }

      await this.notifications.commit(transaction);
    } catch (e) {
      console.log(`Emails Consumer`, e.message, e);
      await this.notifications.rollback(transaction);
    }

    return true;
  }

  /**
   * @param {object} transaction
   * @return {Promise<*>}
   * @protected
   */
  async _getAgentsNotifications(transaction) {
    const filter = new Filter({ sentAt: null, attemptCounterMax: 3 });
    const pagination = new Pagination({ limit: 10 });
    const options = new Options({ transaction, lock: true, skipLocked: true });
    return this.agentNotifications.getAll(filter, pagination, options);
  }

  /**
   * @param {object} agentNotification
   * @return {Promise<void>}
   * @protected
   */
  async _getNotification(agentNotification) {
    const params = new Params({ id: agentNotification.notificationId });
    const notification = await this.notifications.getOne(params);
    assert(!!notification, 'Notification is required');
    return notification;
  }

  /**
   * @param {object} agentNotification
   * @return {Promise<void>}
   * @protected
   */
  async _getAgent(agentNotification) {
    const params = new Params({ id: agentNotification.agentId });
    const agent = await this.agents.getOne(params);
    assert(!!agent, 'Agent is required');
    return agent;
  }

  /**
   * @param {object} agentNotification
   * @param {object} transaction
   * @return {Promise<*>}
   * @protected
   */
  async _markNotificationAsSent(agentNotification, transaction) {
    const params = new Params({ id: agentNotification.id });
    const data = { sentAt: moment.utc().toDate() };
    const options = new Options({ transaction });
    return this.agentNotifications.updateOne(params, data, options);
  }

  /**
   * @param {object} agentNotification
   * @param {CustomError} err
   * @param {object} transaction
   * @return {Promise<*>}
   * @protected
   */
  async _notificationError(agentNotification, err, transaction) {
    const weightError = err.weight || 1;
    const attemptCounterPrev = _.get(agentNotification, 'attemptCounter') || 0;
    const attemptCounter = attemptCounterPrev + weightError;
    const errorAt = moment.utc().toDate();
    const errorReason = err.type;

    const params = new Params({ id: agentNotification.id });
    const data = { errorAt, attemptCounter, errorReason };
    const options = new Options({ transaction });

    return this.agentNotifications.updateOne(params, data, options);
  }

  /**
   * @param {object} agentNotification
   * @return {Promise<*>}
   * @protected
   */
  async _getPreferences(agentNotification) {
    const agentId = agentNotification.agentId;
    const preferences = await this.preferences.getOneByAgentId(agentId);
    assert(!!preferences, 'Preferences is required');
    return preferences;
  }

  /**
   * @param {object} agent
   * @return {Promise<void>}
   * @protected
   */
  async _getUser(agent) {
    const params = new Params({ agentId: agent.id });
    const user = await this.users.getOne(params);
    assert(!!user, `User is required for an agent "${agent.id}".`);
    return user;
  }

  /**
   * @param {object} agentPreferences
   * @param {object} user
   * @return {Promise<boolean>}
   * @protected
   */
  async _test(agentPreferences, user) {
    assert(!!user, 'Users is required');
    assert(!!_.get(user, 'email'), 'User Email is required', errs.EmailAddressError);
    assert(!agentPreferences.stopAllEmails, 'Stop Email Flag is true', errs.PreferencesError);
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
      console.log(`Emails Consumer Emit Email`, e.message, e);
      return false;
    }

    return true;
  }

  /**
   * @param {object} agent
   * @return {Promise<string>}
   * @protected
   */
  async _getAuthToken(agent) {
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
