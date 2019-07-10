const assert = require('assert');
const _ = require('lodash');

const data = require('../../data');

const { TEMPLATE_NAME } = require('../../models/enums/template-name');
const { NOTIFICATION_TYPE } = require('../../models/enums/notification-type');
const { EmailEnvelope } = require('../../models/classes/notifications');

class Producer {
  constructor(options = {}) {
    assert(_.isObject(options), 'Options is not an object');

    this.options = options;
    this.data = data;

    this.TEMPLATE_NAME = TEMPLATE_NAME;
    this.NOTIFICATION_TYPE = NOTIFICATION_TYPE;

    this.EmailEnvelope = EmailEnvelope;
    this.templateName = TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED;
  }

  async _getTemplate() {
    const params = { name: this.templateName };
    return this.data.templates.getOne(params);
  }

  async _createEmailNotification(emailEnvelope) {
    return this._createNotification(emailEnvelope);
  }

  async _createNotification(
    envelope = new this.EmailEnvelope(),
    type = this.NOTIFICATION_TYPE.EMAIL,
  ) {
    const data = { type, envelope: envelope };
    return this.data.notifications.createOne(data);
  }

  async _assignNotification(agentId, notificationId) {
    const data = { agentId, notificationId };
    return this.data.agentNotifications.createOne(data);
  }
}

module.exports = {
  Producer,
};
