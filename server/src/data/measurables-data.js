const _ = require('lodash');

const { notify } = require("../lib/notifications");
const { MeasurableModel } = require('../models-abstract');

const { DataBase } = require('./data-base');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class MeasurablesData extends DataBase {

  constructor() {
    super();
    this.MeasurableModel = new MeasurableModel();
  }

  /**
   * @param data
   * @param {object} user
   * @return {Promise<*>}
   */
  async createOne(data, user) {
    const newMeasurable = await this.models.Measurable.create(data);
    let notification = await newMeasurable.creationNotification(user);
    notify(notification);
    return newMeasurable;
  }

  /**
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async archive(id) {
    let measurable = await this.models.Measurable.findById(id);
    return measurable.archive();
  }

  /**
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async unArchive(id) {
    let measurable = await this.models.Measurable.findById(id);
    return measurable.unarchive();
  }

  /**
   * @param {string} id
   * @param {object} data
   * @param {object} user
   * @return {Promise<Models.Measurable>}
   */
  async updateOne(id, data, user) {
    let measurable = await this.models.Measurable.findById(id);
    let notification = await measurable.updateNotifications(user, data);
    notify(notification);
    return measurable.update(data);
  }

  /**
   * @param {object} options
   * @param {string[]} [options.states]
   * @param {string} [options.agentId]
   * @param {number} [options.offset]
   * @param {number} [options.limit]
   * @return {Promise<*|Array<Model>>}
   */
  async getAll(options) {
    const pagination = {
      offset: options.offset,
      limit: options.limit,
    };
    const restrictions = {
      channelId: true,
      agentId: options.agentId,
    };
    return this.MeasurableModel.getAll(options, pagination, restrictions);
  }

  /**
   * @param {string} id
   * @param {object} options
   * @param {string} options.agentId
   * @return {Promise<*>}
   */
  async getOne(id, options = {}) {
    const restrictions = 'agentId' in options ? {
      channelId: {
        [this.models.sequelize.Op.in]: this.MeasurableModel.channelIdsLiteral(options.agentId)
      }
    } : {};
    return await this.models.Measurable.findOne({
      where: {
        id,
        ...restrictions,
      }
    });
  }

  /**
   * @return {Promise<boolean>}
   */
  needsToBePending() {
    return this.MeasurableModel.needsToBePending();
  }
}

module.exports = {
  MeasurablesData,
};
