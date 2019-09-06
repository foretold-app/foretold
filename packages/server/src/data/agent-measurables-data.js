const _ = require('lodash');

const { AgentMeasurableModel } = require('../models-abstract');
const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../enums/measurement-competitor-type');

const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');
const { MeasurablesData } = require('./measurables-data');
const { Filter } = require('./classes/filter');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentMeasurableModel} AgentMeasurablesModel
 */
class AgentMeasurablesData extends DataBase {

  constructor() {
    super();
    this.AgentMeasurableModel = new AgentMeasurableModel();
    this.model = this.AgentMeasurableModel;
    this.measurements = new MeasurementsData();
    this.measurables = new MeasurablesData();
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
   * Do not make any optimization here, it is early for this.
   * For each optimization we need to do a researching of the performance.
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async primaryPointScore(agentId, measurableId) {
    const {
      predictions,
    } = await this._getMeasurementsToScoring(agentId, measurableId);

    const numCompetitiveMeasurements = _.size(predictions);

    const numAggregatesAfter = predictions
      .map(v => _.size(v.aggregatesAfter))
      .reduce((a, c) => a + c, 0);

    const numAggregatesBefore = predictions
      .map(v => !!v.aggregateBefore ? 1 : 0)
      .reduce((a, c) => a + c, 0);

    return numCompetitiveMeasurements * 10 +
      numAggregatesAfter +
      numAggregatesBefore;
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async _getMeasurementsToScoring(agentId, measurableId) {
    const measurable = await this._getMeasurable(measurableId);

    const {
      recentResult,
      allAggregations,
      proceededPredictions,
    } = await this._getProceededMeasurements(measurableId);

    const predictions = _.filter(
      proceededPredictions,
      ['measurement.agentId', agentId],
    );

    return {
      predictions,
      recentResult,
      allAggregations,
      measurableCreatedAt: measurable.createdAt,
    };
  }

  /**
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async _getProceededMeasurements(measurableId) {
    const measurements = await this._getMeasurements(measurableId);

    const allAggregations = _.filter(measurements, [
      'competitorType', MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
    ]);
    const recentResult = _.find(measurements, [
      'competitorType', MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
    ]);
    const predictions = _.filter(measurements, [
      'competitorType', MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE,
    ]);

    const proceededPredictions = predictions.map((measurement) => {
      const aggregatesAfter = _.filter(allAggregations, (aggregate) => {
        return aggregate.createdAt > measurement.createdAt;
      });
      const aggregateBefore = _.find(allAggregations, (aggregate) => {
        return measurement.createdAt > aggregate.createdAt;
      });
      return {
        measurement,
        aggregatesAfter,
        aggregateBefore,
      };
    });

    return {
      predictions,
      recentResult,
      allAggregations,
      proceededPredictions,
    };
  }

  /**
   * Should return sorted measurements by the createdAt field.
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async _getMeasurements(measurableId) {
    return this.measurements.getAll(new Filter({
      measurableId,
    }));
  }

  /**
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async _getMeasurable(measurableId) {
    return this.measurables.getOne(new Filter({
      id: measurableId,
    }));
  }
}

module.exports = {
  AgentMeasurablesData,
};
