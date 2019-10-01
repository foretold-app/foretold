const assert = require('assert');
const _ = require('lodash');

const data = require('../../data');

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
    this.notifications = data.notifications;
    this.notificationStatuses = data.notificationStatuses;
    this.agents = data.agents;
    this.preferences = data.preferences;
    this.users = data.users;
    this.tokens = data.tokens;
  }
}

module.exports = {
  Consumer,
};
