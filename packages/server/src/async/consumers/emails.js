const assert = require('assert');
const _ = require('lodash');
const moment = require('moment');

const { Consumer } = require('./consumer');

const events = require('../events');
const emitter = require('../emitter');

const { Pagination } = require('../../data/classes/pagination');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');
const { Params } = require('../../data/classes/params');

class Emails extends Consumer {
  constructor() {
    super({});
  }

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

        const notification = await this._getNotification(agentNotification, transaction);
        const agent = await this._getAgent(agentNotification, transaction);
        const agentPreferences = await this._getPreferences(agentNotification);
        const user = await this._getUser(agent);
        const email = user.email;

        await this._emitEmail(notification, agentPreferences, email);

        console.log(
          `\x1b[35mNotification ID = "${notification.id}", ` +
          `Transaction ID = "${transaction.id}", ` +
          `User ID = "${user.id}", ` +
          `Agent Preferences ID = "${agentPreferences.id}", ` +
          `Agent ID = "${agent.id}".\x1b[0m`
        );

        await this._markNotificationAsSent(agentNotification, transaction);
      }

      await this.notifications.commit(transaction);
    } catch (e) {
      console.log(`Emails Consumer`, e.message, e);
    }

    return true;
  }

  async _getAgentsNotifications(transaction) {
    const filter = new Filter({ sentAt: null });
    const pagination = new Pagination({ limit: 10 });
    const options = new Options({ transaction, lock: true, skipLocked: true });
    return this.agentNotifications.getAll(filter, pagination, options);
  }

  async _getNotification(agentNotification) {
    const params = new Params({ id: agentNotification.notificationId });
    const notification = await this.notifications.getOne(params);
    assert(!!notification, 'Notification is required');
    return notification;
  }

  async _getAgent(agentNotification) {
    const params = new Params({ id: agentNotification.agentId });
    const agent = await this.agents.getOne(params);
    assert(!!agent, 'Agent is required');
    return this.agents.getOne(params);
  }

  async _markNotificationAsSent(agentNotification, transaction) {
    const params = new Params({ id: agentNotification.id });
    const data = { sentAt: moment.utc().toDate() };
    const options = new Options({ transaction });
    return this.agentNotifications.updateOne(params, data, options);
  }

  async _getPreferences(agentNotification) {
    const agentId = agentNotification.agentId;
    const preferences = await this.preferences.getOneByAgentId(agentId);
    assert(!!preferences, 'Preferences is required');
    return this.preferences.getOneByAgentId(agentId);
  }

  async _getUser(agent) {
    const params = new Params({ agentId: agent.id });
    const user = await this.users.getOne(params);
    assert(!!user, 'User is required');
    return user;
  }

  async _emitEmail(notification, agentPreferences, email) {
    if (agentPreferences.stopAllEmails) return true;
    if (!!email) return true;

    const envelope = {
      to: notification.envelope.to || email,
      body: notification.envelope.body,
      subject: notification.envelope.subject,
      replacements: {},
    };

    try {
      emitter.emit(events.MAIL, envelope);
    } catch (e) {
      console.log(`Emails Consumer Emit Email`, e.message, e);
    }
    return true;
  }
}

module.exports = {
  Emails,
};
