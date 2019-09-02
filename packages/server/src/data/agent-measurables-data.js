const _ = require('lodash');
const { DataBase } = require('./data-base');

const { AgentMeasurableModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentMeasurableModel} AgentMeasurablesModel
 */
class AgentMeasurablesData extends DataBase {

  constructor() {
    super();
    this.AgentMeasurableModel = new AgentMeasurableModel();
    this.model = this.AgentMeasurableModel;
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoMeasurables(options),
    };
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async primaryPointScore(agentId, measurableId) {
    const measurements = await this.model.getMeasurementsForScoring(
      agentId,
      measurableId,
    );

    const numberOfCompetitiveMeasurements = _.size(measurements) || 0;

    const numberOfAggregatesAfter = measurements
      .map(v => _.size(v.aggregations) || 0)
      .reduce((a, c) => a + c, 0);

    const numberOfAggregatesBefore = measurements
      .map(v => !!v.previousAggregation ? 1 : 0)
      .reduce((a, c) => a + c, 0);

    const result =
      numberOfCompetitiveMeasurements * 10 +
      numberOfAggregatesAfter +
      numberOfAggregatesBefore;

    console.log({
      agentId,
      measurableId
    }, {
      numberOfCompetitiveMeasurements,
      numberOfAggregatesAfter,
      numberOfAggregatesBefore
    }, result);

    return result;
  }
}

module.exports = {
  AgentMeasurablesData,
};
