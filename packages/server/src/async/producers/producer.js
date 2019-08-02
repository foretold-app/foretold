const assert = require('assert');
const _ = require('lodash');

const data = require('../../data');

const { TEMPLATE_NAME } = require('../../models/enums/template-name');
const { NOTIFICATION_TYPE } = require('../../models/enums/notification-type');
const { EmailEnvelope } = require('../../models/classes/notifications');

class Producer {

  constructor(options = {}) {
    assert(_.isObject(options), 'Options is not an object');

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
   * @return {Promise<void>}
   * @protected
   */
  async _getTemplate() {
    assert(!!this.templateName, 'Template Name is required');
    const params = { name: this.templateName };
    return Producer.data.templates.getOne(params);
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
Producer.TEMPLATE_NAME = TEMPLATE_NAME;
Producer.NOTIFICATION_TYPE = NOTIFICATION_TYPE;
Producer.EmailEnvelope = EmailEnvelope;

module.exports = {
  Producer,
};
