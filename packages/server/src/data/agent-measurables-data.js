const _ = require('lodash');
const moment = require('moment');

const {
  PredictionResolutionOverTime,
  marketScore,
} = require('@foretold/prediction-analysis');

const { AgentMeasurableModel } = require('../models-abstract');
const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../enums/measurement-competitor-type');

const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');
const { MeasurablesData } = require('./measurables-data');
const { Filter } = require('./classes');
const logger = require('../lib/log');

const log = logger.module('data/agent-measurables-data');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
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
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurableID} measurableId
   * @returns {Promise<*>}
   */
  async primaryPointScore(agentId, measurableId, startAtFirstPrediction=false) {
    const {
      predictions,
      recentResult,
      allAggregations,
      measurableCreatedAt,
    } = await this._getMeasurementsToScoring(agentId, measurableId);

    function toUnix(r) {
      return moment(r).unix();
    }

    function translateValue(v) {
      let { data, dataType } = v;
      if (dataType === 'percentage') {
        data /= 100;
      }
      return { data, dataType };
    }

    function toOverTime(p) {
      return {
        time: toUnix(p.dataValues.relevantAt),
        measurement: translateValue(p.dataValues.value),
      };
    }

    if (!recentResult) return undefined;
    if (!measurableCreatedAt) return undefined;
    if (predictions.length === 0) return undefined;
    if (allAggregations.length === 0) return undefined;

    const agentPredictions = predictions
      .map((r) => r.measurement)
      .map(toOverTime);
    const marketPredictions = allAggregations.map(toOverTime);
    const resolution = toOverTime(recentResult);

    let overTime;

    const startTime = startAtFirstPrediction ? (agentPredictions[0] && agentPredictions[0].createdAt) : measurableCreatedAt;
    if (!startTime) return undefined;
    try {
      overTime = new PredictionResolutionOverTime({
        agentPredictions,
        marketPredictions,
        resolution,
      }).averagePointScore(marketScore, toUnix(startTime));
    } catch (e) {
      log.trace(e.message);
      return undefined;
    }

    if (!!overTime.error) {
      log.error('PrimaryPointScore Error: ', overTime.error);
      return undefined;
    } if (!_.isFinite(overTime.data)) {
      log.error(
        'Error: PrimaryPointScore score, '
        + '${overTime.data} is not finite',
      );
      return undefined;
    }
    return _.round(overTime.data, 6);
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurableID} measurableId
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
   * @param {Models.MeasurableID} measurableId
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
