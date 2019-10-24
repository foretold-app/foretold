const _ = require('lodash');
const fetch = require('node-fetch');

const { MeasurableModel } = require('../models-abstract');
const { Params } = require('./classes');

const { DataBase } = require('./data-base');
const logger = require('../lib/log');

const log = logger.module('data/measurables-data');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class MeasurablesData extends DataBase {
  constructor() {
    super();
    this.model =  new MeasurableModel();
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
   * @param {Models.MeasurableID} measurableId
   * @return {Promise<Models.Measurable>}
   */
  async unArchive(measurableId) {
    const params = new Params({ id: measurableId });
    const measurable = await this.getOne(params);
    return measurable && measurable.unarchive();
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

  /**
   * @return {Promise<null|boolean|number>}
   */
  async resolutionEndpointResponse(measurable) {
    const endpoint = measurable.resolutionEndpoint;
    if (!endpoint) return false;

    try {
      const response = await fetch(endpoint);
      const json = await response.json();
      const match = JSON.stringify(json).match(/[-+]?[0-9]*\.?[0-9]+/);
      const asFloat = !!match ? parseFloat(match[0]) : null;

      log.trace(
        `Got response from endpoint. Url: ${endpoint}, `
        + `Response: ${JSON.stringify(json)}, Float: ${asFloat}`,
      );

      return asFloat;
    } catch (e) {
      console.error('Error getting response from endpoint. '
        + `Url: ${endpoint}, error: ${e}`);
    }
    return null;
  }
}

module.exports = {
  MeasurablesData,
};
