const assert = require('assert');
const _ = require('lodash')

const data = require('../../data');

const { TEMPLATE_NAME } = require('../../models/enums/template-name');
const { NOTIFICATION_TYPE } = require('../../models/enums/notification-type');
const { EmailEnvelope } = require('../../models/classes/notifications');

/**
 * Here "consumer" class is a parent class of all another "consumers",
 * for instance "EmailsConsumer", "PushConsumer" and another ones.
 */
class Consumer {
  constructor(options = {}) {
    assert(_.isObject(options), 'Options is not an object');

    this.options = options;
    this.data = data;
    this.notifications = data.notifications;
    this.agentNotifications = data.agentNotifications;
    this.agents = data.agents;

    this.TEMPLATE_NAME = TEMPLATE_NAME;
    this.NOTIFICATION_TYPE = NOTIFICATION_TYPE;

    this.EmailEnvelope = EmailEnvelope;
    this.templateName = TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED;
  }
}

module.exports = {
  Consumer,
};
