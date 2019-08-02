const assert = require('assert');
const _ = require('lodash');

const data = require('../../../data');

const { EmailEnvelope } = require('../../../models/classes/notifications');

const { Producer } = require('../producer');

class ProducerNotifications extends Producer {

  constructor(options = {}) {
    super(options);
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
    return await this._createEmailNotification(emailEnvelope$);
  }

  /**
   * @param {object} agent
   * @param {object} notification
   * @return {Promise<*>}
   * @protected
   */
  async _assignAgentToNotification(agent, notification) {
    assert(!!agent.id, 'Agent ID is required');
    assert(!!notification.id, 'Notification ID is required');

    const data = { agentId: agent.id, notificationId: notification.id };
    const options = await this._getOptions();
    return await Producer.data.agentNotifications.createOne(
      data,
      options,
    );
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
}

module.exports = {
  ProducerNotifications,
};
