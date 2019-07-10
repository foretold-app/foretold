const assert = require('assert');
const _ = require('lodash');

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
      const notifications = await this._getNotifications(transaction);

      for (let i = 0; i < notifications.length; i++) {
        const notification = notifications[i];
        this._proceedAgents(notification, transaction);
      }

      await this.notifications.commit(transaction);
    } catch (e) {
      console.log(`Emails Consumer`, e.message, e);
    }
    return true;
  }

  async _proceedAgents(notification, transaction) {
    const agentNotifications = await this._getAgentsNotifications(notification, transaction);
    for (let i = 0; i < agentNotifications.length; i) {
      const agentNotification = agentNotifications[i];
      const agent = await this._getAgent(agentNotification);
    }
  }

  async _getNotifications(transaction) {
    const filter = new Filter({ type: this.NOTIFICATION_TYPE.EMAIL });
    const pagination = new Pagination({ limit: 10 });
    const options = new Options({ transaction });
    return this.notifications.getAll(filter, pagination, options);
  }

  async _getAgentsNotifications(notification, transaction) {
    const filter = new Filter({ notificationId: notification.id });
    const pagination = new Pagination({ limit: 10 });
    const options = new Options({ transaction });
    return this.agentNotifications.getAll(filter, pagination, options);
  }

  async _getAgent(agentNotification) {
    const params = new Params({ id: agentNotification.agentId });
    return this.agents.getOne(params);
  }
}

module.exports = {
  Emails,
};
