const assert = require('assert');

const { Producer } = require('../producer');

/**
 * @abstract
 */
class ProducerNotifications extends Producer {
  constructor(options = {}) {
    super(options);
  }

  /**
   * @param {object} replacements
   * @param {object} to
   * @return {Promise<Models.Notification>}
   * @protected
   */
  async _queueEmail(replacements = {}, to = null) {
    const template = await this._getTemplate();
    const emailEnvelope = new Producer.EmailEnvelope(template.envelopeTemplate);
    const emailEnvelope$ = emailEnvelope
      .instanceFactory(replacements)
      .setTo(to);
    return this._createEmailNotification(emailEnvelope$);
  }

  /**
   * @param {EmailEnvelope} emailEnvelope
   * @return {Promise<Models.Notification>}
   * @protected
   */
  async _createEmailNotification(emailEnvelope) {
    return this._createNotification(emailEnvelope);
  }

  /**
   * @param {EmailEnvelope} envelope
   * @param {string} type
   * @return {Promise<Models.Notification>}
   * @protected
   */
  async _createNotification(
    envelope = new Producer.EmailEnvelope(),
    type = Producer.NOTIFICATION_TYPE.EMAIL,
  ) {
    assert(
      envelope instanceof Producer.EmailEnvelope,
      'Envelope is not EmailEnvelope',
    );
    const data = { type, envelope };
    const options = await this._getOptions();
    return Producer.data.notifications.createOne(data, options);
  }

  /**
   * @param {Models.Agent} agent
   * @param {Models.Notification} notification
   * @return {Promise<Models.AgentNotification>}
   * @protected
   */
  async _assignAgentToNotification(agent, notification) {
    assert(!!agent.id, 'Agent ID is required');
    assert(!!notification.id, 'Notification ID is required');

    const data = { agentId: agent.id, notificationId: notification.id };
    const options = await this._getOptions();
    return Producer.data.agentNotifications.createOne(
      data,
      options,
    );
  }

  /**
   * @param {Models.Notification} notification
   * @return {Promise<Models.AgentNotification>}
   * @protected
   */
  async _assignGuestToNotification(notification) {
    assert(!!notification.id, 'Notification ID is required');

    const data = { notificationId: notification.id };
    const options = await this._getOptions();
    return Producer.data.agentNotifications.createOne(
      data,
      options,
    );
  }
}

module.exports = {
  ProducerNotifications,
};
