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
   * @param {object} [data]
   * @param {Models.Creator} [creator]
   * @return {Promise<Models.Measurable>}
   */
  async createOne(data = {}, creator = {}) {
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
    const measurable = await this.models.Measurable.findByPk(id);
    return measurable.archive();
  }

  /**
   * @todo: rework
   * @public
   * @param {Models.ObjectID} id
   * @return {Promise<Models.Measurable>}
   */
  async unArchive(id) {
    const measurable = await this.models.Measurable.findByPk(id);
    return measurable.unarchive();
  }

  /**
   * @todo: fix interface (params, data, options)
   * @public
   * @param {Models.ObjectID} id
   * @param {object} data
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurable>}
   */
  async updateOne(id, data, creator) {
    const measurable = await this.models.Measurable.findByPk(id);
    notifications.updateNotification(measurable, creator, data);
    return measurable.update(data);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @param {Models.ObjectID} [options.agentId]
   * @param {Models.ObjectID} [options.measuredByAgentId]
   * @return {Promise<{data: Models.Measurable[], total: number}>}
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    const restrictions = {
      channelId: true,
      isAdmin: options.isAdmin,
      agentId: options.agentId,
      measuredByAgentId: options.measuredByAgentId,
    };
    return this.model.getAll(filter, pagination, restrictions);
  }

  /**
   * @todo: move aspects down into Model Layer
   * @todo: fix interface (params*, query, options*)
   * @public
   * @param {Models.ObjectID} id
   * @param {object} options
   * @param {Models.ObjectID} options.agentId
   * @return {Promise<*>}
   */
  async getOne2(id, options = {}) {
    const restrictions = 'agentId' in options ? {
      channelId: {
        [this.model.Op.in]: this.model._channelIdsLiteral(options.agentId),
      },
    } : {};
    return this.models.Measurable.findOne({
      where: {
        id,
        ...restrictions,
      },
    });
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  needsToBePending() {
    return this.model.needsToBePending();
  }

  /**
   * @param {Models.ObjectID} channelId
   * @return {Promise<*>}
   */
  async getOpenedCount(channelId) {
    return this.model.getOpenedCount(channelId);
  }
}

module.exports = {
  MeasurablesData,
};
