const _ = require('lodash');

const notifications = require("../lib/notifications");
const { MeasurableModel } = require('../models-abstract');
const { Restrictions } = require('../models-abstract/classes/restrictions');
const { Params } = require('./classes/params');

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
   * @deprecated: use createOne
   * @param {object} [data]
   * @param {Models.Creator} [creator]
   * @return {Promise<Models.Measurable>}
   */
  async createOne(data = {}, creator = {}) {
    const measurable = await super.createOne(data);
    measurable && notifications.creationNotification(measurable, creator);
    return measurable;
  }

  /**
   * @todo: rework
   * @public
   * @param {string} id
   * @return {Promise<Models.Measurable>}
   */
  async archive(id) {
    const params = new Params({ id });
    const measurable = await this.getOne(params);
    return measurable && measurable.archive();
  }

  /**
   * @todo: rework
   * @public
   * @param {Models.ObjectID} id
   * @return {Promise<Models.Measurable>}
   */
  async unArchive(id) {
    const params = new Params({ id });
    const measurable = await this.getOne(params);
    return measurable && measurable.unarchive();
  }

  /**
   * @todo: fix interface (params, data, options)
   * @public
   * @deprecated: use updateOne
   * @param {Models.ObjectID} id
   * @param {object} data
   * @param {Models.Creator} creator
   * @return {Promise<Models.Measurable>}
   */
  async updateOne(id, data, creator) {
    const params = new Params({ id });
    const measurable = await this.getOne(params);
    measurable && notifications.updateNotification(measurable, creator, data);
    return measurable && measurable.update(data);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @param {Models.ObjectID} [options.agentId]
   * @param {boolean} [options.isAdmin]
   * @param {Models.ObjectID} [options.measuredByAgentId]
   * @return {Promise<{data: Models.Measurable[], total: number}>}
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    const restrictions = new Restrictions({
      channelId: true,
      isAdmin: options.isAdmin,
      agentId: options.agentId,
      // @todo: move to filter
      measuredByAgentId: options.measuredByAgentId,
    });
    return this.model.getAll(filter, pagination, restrictions);
  }

  /**
   * @todo: move aspects down into Model Layer
   * @todo: fix interface (params*, query, options*)
   * @public
   * @deprecated: use getOne
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
   * @return {Promise<Models.Measurable[]>}
   */
  async needsToBePending() {
    return this.model.needsToBePending();
  }

  /**
   * @public
   * @return {Promise<Models.Measurable[]>}
   */
  async needsResolutionResponse() {
    return this.model.needsResolutionResponse();
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
