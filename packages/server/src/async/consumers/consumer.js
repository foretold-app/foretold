const assert = require('assert');
const _ = require('lodash')

const data = require('../../data');

class Consumer {
  constructor(options = {}) {
    assert(_.isObject(options), 'Options is not an object');

    this.options = options;
    this.data = data;
    this.notifications = data.notifications;
    this.agentNotifications = data.agentNotifications;
    this.agents = data.agents;
    this.preferences = data.preferences;
    this.users = data.users;
  }
}

module.exports = {
  Consumer,
};
