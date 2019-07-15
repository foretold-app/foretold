const assert = require('assert');
const _ = require('lodash');

const data = require('../../data');

const { TEMPLATE_NAME } = require('../../models/enums/template-name');
const { NOTIFICATION_TYPE } = require('../../models/enums/notification-type');
const { EmailEnvelope } = require('../../models/classes/notifications');

/**
 * Here "producer" class is a parent class of all another "producers",
 * for instance "MeasurableStateProducer", and another ones.
 */
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
    assert(envelope instanceof this.EmailEnvelope, 'Envelope is not EmailEnvelope');
    const data = { type, envelope: envelope };
    return this.data.notifications.createOne(data);
  }

  async _assignNotification(agentId, notificationId) {
    const data = { agentId, notificationId };
    return this.data.agentNotifications.createOne(data);
  }

  async _queueEmail(agent) {
    const template = await this._getTemplate();
    assert(!!_.get(template, 'id'), 'Template ID is required');
    assert(!!_.get(template, 'envelopeTemplate'), 'Envelope Template ID is required');

    const emailEnvelope = new this.EmailEnvelope(template.envelopeTemplate);
    const notification = await this._createEmailNotification(emailEnvelope);
    assert(!!_.get(notification, 'id'), 'Notification ID is required');

    const assignment = await this._assignNotification(agent.id, notification.id);
    assert(!!_.get(assignment, 'id'), 'Assignment ID is required');

    return assignment;
  }
}

module.exports = {
  Producer,
};
