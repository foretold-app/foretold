const assert = require('assert');
const _ = require('lodash');

const data = require('../../data');
const { MeasurementsData } = require('../../data');
const { UsersData } = require('../../data');
const { AgentsData } = require('../../data');
const { ChannelsData } = require('../../data');
const { NotificationsData } = require('../../data');
const { NotificationStatusesData } = require('../../data');

const { TEMPLATE_NAME } = require('../../enums');
const { NOTIFICATION_TYPE } = require('../../enums');
const { EmailEnvelope } = require('../../models/classes/notifications');

const { FeedItem } = require('../../models/classes');
const { FeedItemGeneric } = require('../../models/classes');
const { FeedItemMeasurable } = require('../../models/classes');

/**
 * @abstract
 */
class Producer {
  constructor(options = {}) {
    assert(_.isObject(options), 'Options is not an object');

    this.measurements = new MeasurementsData();
    this.users = new UsersData();
    this.agents = new AgentsData();
    this.channels = new ChannelsData();
    this.notifications = new NotificationsData();
    this.notificationStatuses = new NotificationStatusesData();

    this.options = options;
    this.templateName = undefined;
    this.transaction = undefined;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    return true;
  }

  /**
   * @return {Promise<undefined>}
   * @protected
   */
  async _getTransaction() {
    if (!!this.transaction) return this.transaction;
    this.transaction = await Producer.data.notifications.getTransaction();
    return this.transaction;
  }

  /**
   * @return {Promise<object>}
   * @protected
   */
  async _commit() {
    const transaction = await this._getTransaction();
    return Producer.data.notifications.commit(transaction);
  }

  /**
   * @return {Promise<object>}
   * @protected
   */
  async _rollback() {
    const transaction = await this._getTransaction();
    return Producer.data.notifications.rollback(transaction);
  }

  /**
   * @return {Promise<Models.Template>}
   * @protected
   */
  async _getTemplate() {
    assert(!!this.templateName, 'Template Name is required');
    const params = { name: this.templateName };
    const template = await Producer.data.templates.getOne(params);

    assert(!!_.get(template, 'name'), 'Template name is required');
    assert(
      !!_.get(template, 'envelopeTemplate'),
      'Envelope Template ID is required',
    );

    return template;
  }

  /**
   * @return {Promise<{transaction: *}>}
   * @protected
   */
  async _getOptions() {
    const transaction = await this._getTransaction();
    return { transaction };
  }
}

Producer.data = data;
Producer.TEMPLATE_NAME = TEMPLATE_NAME;
Producer.NOTIFICATION_TYPE = NOTIFICATION_TYPE;
Producer.EmailEnvelope = EmailEnvelope;

Producer.FeedItem = FeedItem;
Producer.FeedItemGeneric = FeedItemGeneric;
Producer.FeedItemMeasurable = FeedItemMeasurable;

module.exports = {
  Producer,
};
