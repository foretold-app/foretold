const assert = require('assert');
const _ = require('lodash');
const moment = require('moment');

const { Consumer } = require('./consumer');

const { Pagination } = require('../../data/classes/pagination');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');
const { Params } = require('../../data/classes/params');
const { Query } = require('../../data/classes/query');

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

        console.log(
          `\x1b[35mNotification ID = "${notification.id}", ` +
          `Transaction ID = "${transaction.id}", ` +
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
    const pagination = new Pagination({ limit: 1 });
    const options = new Options({ transaction, lock: true, skipLocked: true });
    return this.agentNotifications.getAll(filter, pagination, options);
  }

  async _getNotification(agentNotification, transaction) {
    const params = new Params({ id: agentNotification.notificationId });
    const query = new Query();
    const options = new Options({ transaction });
    return this.notifications.getOne(params, query, options);
  }

  async _getAgent(agentNotification, transaction) {
    const params = new Params({ id: agentNotification.agentId });
    const query = new Query();
    const options = new Options({ transaction });
    return this.agents.getOne(params, query, options);
  }

  async _markNotificationAsSent(agentNotification, transaction) {
    const params = new Params({ id: agentNotification.id });
    const data = { sentAt: moment.utc().toDate() };
    const options = new Options({ transaction });
    return this.agentNotifications.updateOne(params, data, options);
  }
}

module.exports = {
  Emails,
};
