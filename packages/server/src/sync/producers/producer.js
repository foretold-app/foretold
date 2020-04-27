const assert = require('assert');
const _ = require('lodash');

const { MeasurementsData } = require('../../data');
const { UsersData } = require('../../data');
const { AgentsData } = require('../../data');
const { ChannelsData } = require('../../data');
const { NotificationsData } = require('../../data');
const { NotificationStatusesData } = require('../../data');
const { TemplatesData } = require('../../data');
const { FeedItemsData } = require('../../data');

const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');

const { TEMPLATE_NAME } = require('../../enums');
const { NOTIFICATION_TYPE } = require('../../enums');
const { EmailEnvelope } = require('../../data/models/classes/notifications');

const { FeedItem } = require('../../data/models/classes');
const { FeedItemGeneric } = require('../../data/models/classes');
const { FeedItemMeasurable } = require('../../data/models/classes');
const { FeedItemMeasurement } = require('../../data/models/classes');
const { FeedItemJoinedMember } = require('../../data/models/classes');
const { FeedItemChannel } = require('../../data/models/classes');
const { FeedItemNotebook } = require('../../data/models/classes');
const { FeedItemSeries } = require('../../data/models/classes');

/**
 * @abstract
 */
class Producer {
  constructor(options = {}) {
    assert(_.isObject(options), 'Options is not an object.');

    this.measurements = new MeasurementsData();
    this.users = new UsersData();
    this.agents = new AgentsData();
    this.channels = new ChannelsData();
    this.notifications = new NotificationsData();
    this.notificationStatuses = new NotificationStatusesData();
    this.templates = new TemplatesData();
    this.feedItems = new FeedItemsData();

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
    this.transaction = await this.notifications.getTransaction();
    return this.transaction;
  }

  /**
   * @return {Promise<object>}
   * @protected
   */
  async _commit() {
    const transaction = await this._getTransaction();
    return this.notifications.commit(transaction);
  }

  /**
   * @return {Promise<object>}
   * @protected
   */
  async _rollback() {
    const transaction = await this._getTransaction();
    return this.notifications.rollback(transaction);
  }

  /**
   * @param {string} [name]
   * @return {Promise<Models.Template>}
   * @protected
   */
  async _getTemplate(name = this.templateName) {
    assert(!!name, 'Template Name is required.');

    const params = new Params({ name });
    const template = await this.templates.getOne(params);

    assert(!!_.get(template, 'name'), 'Template name is required.');
    assert(
      !!_.get(template, 'envelopeTemplate'),
      'Envelope Template ID is required.',
    );

    return template;
  }

  /**
   * @return {Promise<{transaction: *}>}
   * @protected
   */
  async _getOptions() {
    const transaction = await this._getTransaction();
    return new Options({ transaction });
  }
}

Producer.TEMPLATE_NAME = TEMPLATE_NAME;
Producer.NOTIFICATION_TYPE = NOTIFICATION_TYPE;
Producer.EmailEnvelope = EmailEnvelope;

Producer.FeedItem = FeedItem;
Producer.FeedItemGeneric = FeedItemGeneric;
Producer.FeedItemMeasurable = FeedItemMeasurable;
Producer.FeedItemMeasurement = FeedItemMeasurement;
Producer.FeedItemJoinedMember = FeedItemJoinedMember;
Producer.FeedItemChannel = FeedItemChannel;
Producer.FeedItemNotebook = FeedItemNotebook;
Producer.FeedItemSeries = FeedItemSeries;

module.exports = {
  Producer,
};
