const assert = require('assert');
const _ = require('lodash');
const moment = require('moment');

const { Consumer } = require('./consumer');

const { Pagination } = require('../../data/classes/pagination');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');
const { Params } = require('../../data/classes/params');

class Emails extends Consumer {
  constructor() {
    super({});
  }

  async main() {
    try {
      const transaction = await this.notifications.getTransaction();
      const agentNotifications = await this._getAgentsNotifications(transaction);

      for (let i = 0; i < agentNotifications.length; i++) {
        const agentNotification = agentNotifications[i];

        const notification = await this._getNotification(agentNotification);
        const agent = await this._getAgent(agentNotification);

        console.log(
          `Notification ID = "${notification.id}", ` +
          `Agent ID = "${agent.id}".`
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
    const options = new Options({ transaction });
    return this.agentNotifications.getAll(filter, pagination, options);
  }

  async _getNotification(agentNotification) {
    const params = new Params({ id: agentNotification.notificationId });
    return this.notifications.getOne(params);
  }

  async _getAgent(agentNotification) {
    const params = new Params({ id: agentNotification.agentId });
    return this.agents.getOne(params);
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
