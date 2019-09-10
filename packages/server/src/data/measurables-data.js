const _ = require('lodash');

const notifications = require('../lib/notifications');
const { MeasurableModel } = require('../models-abstract');
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
   * @public
   * @param {Models.Measurable} measurable
   * @return {Promise<Models.Measurable>}
   */
  async processResolution(measurable) {
    return this.model.processResolution(measurable);
  }

  /**
   * @param {Models.ObjectID} channelId
   * @return {Promise<*>}
   */
  async getOpenedCount(channelId) {
    return this.model.getOpenedCount(channelId);
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      channelId: true,
      isAdmin: options.isAdmin,
      agentId: options.agentId,
      // @todo: move to filter
      measuredByAgentId: options.measuredByAgentId,
    };
  }
}

module.exports = {
  MeasurablesData,
};
