const _ = require('lodash');
const assert = require('assert');

const { Producer } = require('../producer');

const { Data } = require('../../../data/classes');

/**
 * @abstract
 */
class ProducerNotifications extends Producer {
  constructor(options = {}) {
    super(options);
  }

  /**
   * @param {object} replacements
   * @param {string | null} recipient
   * @return {Promise<Models.Notification>}
   * @protected
   */
  async _queueEmail(replacements = {}, recipient = null) {
    const template = await this._getTemplate(this.templateName);
    const outerTemplate = await this._getOuterTemplate();

    const emailEnvelope = new Producer.EmailEnvelope(template.envelopeTemplate);
    const emailEnvelope$ = emailEnvelope
      .setRecipient(recipient)
      .setOuterTemplate(outerTemplate)
      .instanceFactory(replacements);

    return this._createEmailNotification(emailEnvelope$);
  }

  /**
   * @returns {Promise<string>}
   * @protected
   */
  async _getOuterTemplate() {
    const name = Producer.TEMPLATE_NAME.EMAIL_OUTER_TEMPLATE;
    const outerTemplate = await this._getTemplate(name);
    return _.get(outerTemplate, 'envelopeTemplate.outerTemplate') || '';
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
      'Envelope is not EmailEnvelope.',
    );
    const data = new Data({ type, envelope });
    const options = await this._getOptions();
    return this.notifications.createOne(data, options);
  }

  /**
   * @param {Defs.Agent} agent
   * @param {Defs.Notification} notification
   * @return {Promise<Models.NotificationStatus>}
   * @protected
   */
  async _assignNotification(agent, notification) {
    assert(!!_.get(notification, 'id'), 'Notification ID is required.');

    const data = new Data({
      agentId: _.get(agent, 'id', null),
      notificationId: notification.id,
    });

    const options = await this._getOptions();
    return this.notificationStatuses.createOne(
      data,
      options,
    );
  }
}

module.exports = {
  ProducerNotifications,
};
