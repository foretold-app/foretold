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
    this.templateName = undefined;
  }

  /**
   * @param {object} replacements
   * @return {Promise<*>}
   * @protected
   */
  async _queueEmail(replacements) {
    const template = await this._getTemplate();
    assert(!!_.get(template, 'id'), 'Template ID is required');
    assert(
      !!_.get(template, 'envelopeTemplate'),
      'Envelope Template ID is required',
    );

    const emailEnvelope = new Producer.EmailEnvelope(template.envelopeTemplate);
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
    const assignment = await Producer.data.agentNotifications.createOne(data);
    assert(!!_.get(assignment, 'id'), 'Assignment ID is required');
    return assignment;
  }

  /**
   * @return {Promise<void>}
   * @protected
   */
  async _getTemplate() {
    assert(!!this.templateName, 'Template Name is required');
    const params = { name: this.templateName };
    return Producer.data.templates.getOne(params);
  }

  /**
   * @param {Producer.EmailEnvelope} emailEnvelope
   * @return {Promise<*>}
   * @protected
   */
  async _createEmailNotification(emailEnvelope) {
    return this._createNotification(emailEnvelope);
  }

  /**
   * @param {Producer.EmailEnvelope} envelope
   * @param {string} type
   * @return {Promise<*>}
   * @protected
   */
  async _createNotification(
    envelope = new Producer.EmailEnvelope(),
    type = Producer.NOTIFICATION_TYPE.EMAIL,
  ) {
    assert(
      envelope instanceof Producer.EmailEnvelope,
      'Envelope is not EmailEnvelope'
    );
    const data = { type, envelope: envelope };
    return Producer.data.notifications.createOne(data);
  }
}

Producer.data = data;
Producer.TEMPLATE_NAME = TEMPLATE_NAME;
Producer.TEMPLATE_NAME = TEMPLATE_NAME;
Producer.NOTIFICATION_TYPE = NOTIFICATION_TYPE;
Producer.EmailEnvelope = EmailEnvelope;

module.exports = {
  Producer,
};
