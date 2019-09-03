const _ = require('lodash');

const { AgentMeasurableModel } = require('../models-abstract');
const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../models/enums/measurement-competitor-type');

const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');
const { MeasurablesData } = require('./measurables-data');
const { Filter } = require('./classes/filter');
const { Params } = require('./classes/params');

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
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async primaryPointScore(agentId, measurableId) {

    // const measurements = await this.model.getMeasurementsForScoring(
    //   agentId,
    //   measurableId,
    // );
    //
    // const numberOfCompetitiveMeasurements = _.size(measurements);
    //
    // const numberOfAggregatesAfter = measurements
    //   .map(v => _.size(v.aggregations))
    //   .reduce((a, c) => a + c, 0);
    //
    // const numberOfAggregatesBefore = measurements
    //   .map(v => !!v.previousAggregation ? 1 : 0)
    //   .reduce((a, c) => a + c, 0);
    //
    // const result =
    //   numberOfCompetitiveMeasurements * 10 +
    //   numberOfAggregatesAfter +
    //   numberOfAggregatesBefore;
    //
    // console.log({
    //   agentId,
    //   measurableId
    // }, {
    //   numberOfCompetitiveMeasurements,
    //   numberOfAggregatesAfter,
    //   numberOfAggregatesBefore
    // }, result);

    return 0;
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async _getMeasurementsToScoring(agentId, measurableId) {
    const measurable = await this._getMeasurable(measurableId);

    const {
      allAggregations,
      recentResult,
      predictions,
    } = await this._getPrefetchedMeasurements(measurableId);

    const proceededPredictions = predictions
      .filter(measurement => measurement.agentId === agentId);

    return proceededPredictions;
  }



  /**
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async _getPrefetchedMeasurements( measurableId) {
    const measurements = this._getMeasurements(measurableId);

    const allAggregations = _.filter(measurements, [
      'competitorType', MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
    ]);
    const recentResult = _.find(measurements, [
      'competitorType', MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
    ]);
    const predictions = _.filter(measurements, [
      'competitorType', MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE,
    ]);

    const proceededPredictions = predictions
      .map((measurement) => {
        const aggregatesAfter = _.filter(allAggregations, (aggregate) => {
          return aggregate.createdAt > measurement.createdAt;
        });
        const aggregateBefore = _.find(allAggregations, (aggregate) => {
          return measurement.createdAt > aggregate.createdAt;
        });
        return {
          aggregatesAfter,
          aggregateBefore,
          measurement,
        };
      });

    return {
      allAggregations,
      recentResult,
      predictions,
      proceededPredictions,
    };
  }



  /**
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async _getMeasurements(measurableId) {
    const measurements = this.measurements.getAll(new Filter({
      measurableId,
    }));
    return measurements;
  }

  /**
   * @param {Models.ObjectID} measurableId
   * @returns {Promise<*>}
   */
  async _getMeasurable(measurableId) {
    const measurable = this.measurables.getOne(new Filter({
      id: measurableId,
    }));
    return measurable;
  }
}

module.exports = {
  AgentMeasurablesData,
};
