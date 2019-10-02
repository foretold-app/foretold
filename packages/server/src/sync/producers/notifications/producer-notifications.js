const assert = require('assert');
const _ = require('lodash');

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
   * @return {Promise<Models.NotificationStatus>}
   * @protected
   */
  async _assignNotification(agent, notification) {
    assert(!!notification.id, 'Notification ID is required');

    const data = {
      agentId: _.get(agent, 'id', null),
      notificationId: notification.id,
    };

    const options = await this._getOptions();
    return Producer.data.notificationStatuses.createOne(
      data,
      options,
    );
  }
}

module.exports = {
  ProducerNotifications,
};
