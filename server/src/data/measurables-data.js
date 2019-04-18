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
   * @param {object} data
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurable>}
   */
  async createOne(data, creator) {
    const measurable = await this.models.Measurable.create(data);
    /** @type {Models.Measurable} */
    const notification = await measurable.creationNotification(creator);
    notify(notification);
    return measurable;
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
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurable>}
   */
  async updateOne(id, data, creator) {
    let measurable = await this.models.Measurable.findById(id);
    /** @type {Models.Measurable} */
    const notification = await measurable.updateNotifications(creator, data);
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
