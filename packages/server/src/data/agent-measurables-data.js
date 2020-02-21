const _ = require('lodash');

const { AgentMeasurableModel } = require('../models');

const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');
const { MeasurablesData } = require('./measurables-data');

const { Proceed } = require('./scoring/proceed');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {AgentMeasurableModel} model
 */
class AgentMeasurablesData extends DataBase {
  constructor() {
    super();
    this.model = new AgentMeasurableModel();
    this.measurements = new MeasurementsData();
    this.measurables = new MeasurablesData();
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelRestrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoMeasurables(options),
    };
  }

  /**
   * Do not make any optimization here, it is early for this.
   * For each optimization we need to do a researching of the performance.
   * @param agentId
   * @param measurableId
   * @param params
   * @returns {Promise<undefined|
   * {
   * score: *,
   * agentPredictions: *,
   * finalResolutionTime: *,
   * scoringStartTime: *,
   * timeActivityRatio: *,
   * recentResult: *,
   * activeTimeDistribution: {finalX: *, points: *},
   * measurableCreationTime: *,
   * aggregations: *,
   * scoringEndTime: *
   * }>}
   */
  async primaryPointScore(agentId, measurableId, params) {
    const item = await this._getTimeScoringData(agentId, measurableId);
    return new Proceed().primaryPointScore2(item, params);
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurableID} measurableId
   * @returns {Promise<*>}
   */
  async _getTimeScoringData(agentId, measurableId) {
    const response = await this.model.scoringQuery(agentId, measurableId);
    return _.head(response);
  }
}

module.exports = {
  AgentMeasurablesData,
};
