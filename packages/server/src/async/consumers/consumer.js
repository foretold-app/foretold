const assert = require('assert');
const _ = require('lodash')

const data = require('../../data');

const { TEMPLATE_NAME } = require('../../models/enums/template-name');
const { NOTIFICATION_TYPE } = require('../../models/enums/notification-type');
const { EmailEnvelope } = require('../../models/classes/notifications');

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
