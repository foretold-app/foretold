const assert = require('assert');
const _ = require('lodash');

const data = require('../../data');
const { NotificationsData } = require('../../data');
const { NotificationStatusesData } = require('../../data');
const { AgentsData } = require('../../data');
const { PreferencesData } = require('../../data');
const { UsersData } = require('../../data');
const { TokensData } = require('../../data');

/**
 * Here "consumer" class is a parent class of all another "consumers",
 * for instance "EmailsConsumer", "PushConsumer" and another ones.
 * @abstract
 */
class Consumer {
  constructor(options = {}) {
    assert(_.isObject(options), 'Options is not an object');

    this.options = options;
    this.data = data;
    this.notifications = new NotificationsData();
    this.notificationStatuses = new NotificationStatusesData();
    this.agents = new AgentsData();
    this.preferences = new PreferencesData();
    this.users = new UsersData();
    this.tokens = new TokensData();
  }
}

module.exports = {
  Consumer,
};
