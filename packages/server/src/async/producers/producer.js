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
    this.transaction = undefined;
  }

  /**
   * @return {Promise<undefined>}
   * @protected
   */
  async _getTransaction() {
    if (!!this.transaction) return this.transaction;
    this.transaction = await Producer.data.users.getTransaction();
    return this.transaction;
  }

  /**
   * @return {Promise<object>}
   * @protected
   */
  async _commit() {
    const transaction = await this.transaction;
    return Producer.data.users.commit(transaction);
  }

  /**
   * @return {Promise<object>}
   * @protected
   */
  async _rollback() {
    const transaction = await this.transaction;
    return Producer.data.users.rollback(transaction);
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
    const options = await this._getOptions();
    const assignment = await Producer.data.agentNotifications.createOne(
      data,
      options,
    );
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
   * @param {EmailEnvelope} envelope
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
    const options = await this._getOptions();
    return Producer.data.notifications.createOne(data, options);
  }

  /**
   *
   * @return {Promise<{transaction: *}>}
   * @private
   */
  async _getOptions() {
    const transaction = await this._getTransaction();
    return { transaction };
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
