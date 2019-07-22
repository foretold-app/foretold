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

  /**
   * @param {object} replacements
   * @return {Promise<*>}
   * @protected
   */
  async _queueEmail(replacements) {
    const template = await this._getTemplate();
    assert(!!_.get(template, 'id'), 'Template ID is required');
    assert(!!_.get(template, 'envelopeTemplate'), 'Envelope Template ID is required');

    const emailEnvelope = new this.EmailEnvelope(template.envelopeTemplate);
    const emailEnvelope$ = emailEnvelope.mutate(replacements);
    const notification = await this._createEmailNotification(emailEnvelope$);
    assert(!!_.get(notification, 'id'), 'Notification ID is required');

    return notification;
  }

  /**
   * @param {object} agent
   * @param {object} notification
   * @return {Promise<*>}
   * @protected
   */
  async _assignNotification(agent, notification) {
    const data = { agentId: agent.id, notificationId: notification.id };
    const assignment = await this.data.agentNotifications.createOne(data);
    assert(!!_.get(assignment, 'id'), 'Assignment ID is required');
    return assignment;
  }

  /**
   * @return {Promise<void>}
   * @protected
   */
  async _getTemplate() {
    const params = { name: this.templateName };
    return this.data.templates.getOne(params);
  }

  /**
   * @param emailEnvelope
   * @return {Promise<*>}
   * @protected
   */
  async _createEmailNotification(emailEnvelope) {
    return this._createNotification(emailEnvelope);
  }

  /**
   * @param envelope
   * @param type
   * @return {Promise<*>}
   * @protected
   */
  async _createNotification(
    envelope = new this.EmailEnvelope(),
    type = this.NOTIFICATION_TYPE.EMAIL,
  ) {
    assert(envelope instanceof this.EmailEnvelope, 'Envelope is not EmailEnvelope');
    const data = { type, envelope: envelope };
    return this.data.notifications.createOne(data);
  }
}

module.exports = {
  Producer,
};
