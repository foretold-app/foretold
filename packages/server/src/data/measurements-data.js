const { DataBase } = require('./data-base');

const { MeasurementModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {MeasurementModel} MeasurementModel
 */
class MeasurementsData extends DataBase {
  constructor() {
    super();
    this.model = new MeasurementModel();
  }

  /**
   * @public
   * @param {*} options
   * @return {Promise<Models.Model>}
   */
  async getLatest(options) {
    return this.model.getLatest(options);
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      measurableId: true,
      isAdmin: options.isAdmin,
      agentId: options.agentId,
    };
  }

  /**
   * @public
   * @param {Models.ObjectID | null} measurableId
   * @returns {Promise<Model>}
   */
  async getOutcome(measurableId) {
    return this.model.getOutcome(measurableId);
  }

  /**
   * @public
   * @param {Models.ObjectID | null} measurableId
   * @param {Models.ObjectID | null} agentId
   * @param {Date} relevantAt
   * @returns {Promise<Model>}
   */
  async getPreviousRelevantAggregate(measurableId, agentId, relevantAt) {
    return this.model.getPreviousRelevantAggregate(
      measurableId,
      agentId,
      relevantAt,
    );
  }

  /**
   * @public
   * @param {Models.ObjectID | null} measurableId
   * @param {Models.ObjectID | null} agentId
   * @returns {Promise<Model>}
   */
  async getLatestAggregate(measurableId, agentId) {
    return this.model.getLatestAggregate(measurableId, agentId);
  }
}

module.exports = {
  MeasurementsData,
};
