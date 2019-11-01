const _ = require('lodash');
const moment = require('moment');
const { MARKET_TYPE, START_AT, FINAL_COMPARISON_MEASUREMENT } = require('../enums/agent-measurable-score-type');

const {
  PredictionResolutionOverTime,
  marketScore,
  nonMarketScore
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

function toUnix(r) {
  return moment(r).unix();
}


function timeActivityRatio({ initialTime, firstPredictionTime, endingTime }) {
  if (!initialTime || !firstPredictionTime || !endingTime) {
    return undefined;
  }
  const _initialTime = toUnix(initialTime);
  const _firstPredictionTime = toUnix(firstPredictionTime);
  const _endingTime = toUnix(endingTime);
  return (_endingTime - _firstPredictionTime) / (_endingTime - _initialTime);
}

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
  async primaryPointScore(agentId, measurableId, params = {
    marketType: MARKET_TYPE.MARKET,
    startAt: START_AT.QUESTION_CREATION_TIME,
    finalComparisonMeasurement: FINAL_COMPARISON_MEASUREMENT.LAST_OBJECTIVE_MEASUREMENT,
  }) {
    const {
      agentPredictions,
      recentResult,
      allAggregations,
      measurable,
    } = await this._getTimeScoringData(agentId, measurableId);

    console.log("11111111111111111", agentPredictions.length, allAggregations.length, !!recentResult, !!measurable)
    // Checks ----------------------------------------------------------------------
    if (agentPredictions.length === 0) return undefined;
    if (allAggregations.length === 0) return undefined;
    if (!recentResult) return undefined;
    if (!measurable) return undefined;

    // Use of Parameters ----------------------------------------------------------------------
    const resolutionMeasurement = (params.finalComparisonMeasurement === FINAL_COMPARISON_MEASUREMENT.LAST_OBJECTIVE_MEASUREMENT)
      ? recentResult
      : _.last(allAggregations);

    if (!resolutionMeasurement) return undefined;

    const startTime = (params.startAt === START_AT.QUESTION_CREATION_TIME)
      ? (measurable.createdAt)
      : (!!agentPredictions[0] && agentPredictions[0].dataValues.relevantAt);

    const marketScoreType = params.marketType === MARKET_TYPE.MARKET ? marketScore : nonMarketScore;

    const timeScore = this._scoreCalculator({
      agentPredictions,
      allAggregations,
      resolutionMeasurement,
      marketScoreType,
      startTime,
    });

    const _timeActivityRatio = timeActivityRatio({
      initialTime: measurable.createdAt,
      firstPredictionTime: agentPredictions[0] && agentPredictions[0].dataValues.relevantAt,
      endingTime: resolutionMeasurement.dataValues.relevantAt,
    });

    return {
      score: _.round(timeScore, 6),
      agentPredictions,
      aggregations: allAggregations,
      recentResult,
      scoringStartTime: startTime,
      scoringEndTime: resolutionMeasurement.dataValues.relevantAt,
      measurableCreationTime: measurable.createdAt,
      finalResolutionTime: resolutionMeasurement.dataValues.relevantAt,
      timeActivityRatio: _.round(_timeActivityRatio, 6),
    };
  }

  _scoreCalculator({
    agentPredictions,
    allAggregations,
    resolutionMeasurement,
    marketScoreType,
    startTime,
  }) {
    // Private Functions -----------------------------------------------------

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

    // Main Function -------------------------------------------------------
    let timeScore;
    if (!startTime) return undefined;
    try {
      timeScore = new PredictionResolutionOverTime({
        agentPredictions: agentPredictions.map(toOverTime),
        marketPredictions: allAggregations.map(toOverTime),
        resolution: toOverTime(resolutionMeasurement),
      }).averagePointScore(marketScoreType, toUnix(startTime));
    } catch (e) {
      log.trace(e.message);
      return undefined;
    }

    if (!!timeScore.error) {
      log.error('PrimaryPointScore Error: ', timeScore.error);
      return undefined;
    } if (!_.isFinite(timeScore.data)) {
      log.error(
        'Error: PrimaryPointScore score, '
        + '${timeScore.data} is not finite',
      );
      return undefined;
    }
    return timeScore.data;
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurableID} measurableId
   * @returns {Promise<*>}
   */
  async _getTimeScoringData(agentId, measurableId) {
    const measurable = await this._getMeasurable(measurableId);

    const measurements = await this._getMeasurements(measurableId);

    const allAggregations = _.filter(measurements, [
      'competitorType', MEASUREMENT_COMPETITOR_TYPE.AGGREGATION,
    ]);

    const recentResult = _.find(measurements, [
      'competitorType', MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
    ]);

    const agentPredictions = _.filter(measurements, {
      competitorType: MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE,
      agentId,
    });

    return {
      agentPredictions,
      recentResult,
      allAggregations,
      measurable,
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
