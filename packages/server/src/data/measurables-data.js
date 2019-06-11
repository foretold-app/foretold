const _ = require('lodash');

const notifications = require("../lib/notifications");
const { MeasurableModel } = require('../models-abstract');

const { DataBase } = require('./data-base');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class MeasurablesData extends DataBase {

  constructor() {
    super();
    this.MeasurableModel = new MeasurableModel();
    this.model = this.MeasurableModel;
  }

  /**
   * @todo: fix interface (data, options)
   * @public
   * @param {object} data
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurable>}
   */
  async createOne(data, creator) {
    const measurable = await this.models.Measurable.create(data);
    notifications.creationNotification(measurable, creator);
    return measurable;
  }

  /**
   * @todo: rework
   * @public
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async archive(id) {
    const measurable = await this.models.Measurable.findById(id);
    return measurable.archive();
  }

  /**
   * @todo: rework
   * @public
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async unArchive(id) {
    const measurable = await this.models.Measurable.findById(id);
    return measurable.unarchive();
  }

  /**
   * @todo: fix interface (params, data, options)
   * @public
   * @param {string} id
   * @param {object} data
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurable>}
   */
  async updateOne(id, data, creator) {
    const measurable = await this.models.Measurable.findById(id);
    notifications.updateNotification(measurable, creator, data);
    return measurable.update(data);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @param {string} [options.agentId]
   * @param {string} [options.measuredByAgentId]
   * @return {Promise<{data: Models.Measurable[], total: number}>}
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    const restrictions = {
      channelId: true,
      isAdmin: options.isAdmin,
      agentId: options.agentId,
      measuredByAgentId: options.measuredByAgentId,
    };
    return await this.MeasurableModel.getAll(filter, pagination, restrictions);
  }

  /**
   * @todo: move down
   * @todo: fix interface (params*, query, options*)
   * @public
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
   * @public
   * @return {Promise<boolean>}
   */
  needsToBePending() {
    return this.MeasurableModel.needsToBePending();
  }
}

module.exports = {
  MeasurablesData,
};
