// Third-part dependencies
const _ = require('lodash');
const moment = require('moment');
const {
  PredictionResolutionOverTime,
  marketScore,
  nonMarketScore,
} = require('@foretold/prediction-analysis');

// The first level of dependencies (..)
const logger = require('../lib/log');
const { AgentMeasurableModel } = require('../models-abstract');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../enums');
const {
  MARKET_TYPE,
  START_AT,
  FINAL_COMPARISON_MEASUREMENT,
} = require('../enums');

// The second level (.)
const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');
const { MeasurablesData } = require('./measurables-data');
const { Filter } = require('./classes');
const { Params } = require('./classes');

const log = logger.module('data/agent-measurables-data');

const { LAST_OBJECTIVE_MEASUREMENT } = FINAL_COMPARISON_MEASUREMENT;

/**
 * @param r
 * @returns {number}
 */
function toUnix(r) {
  return moment(r).unix();
}

/**
 * @param initialTime
 * @param firstPredictionTime
 * @param endingTime
 * @returns {number|undefined}
 */
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
  async primaryPointScore(agentId, measurableId, params = {
    marketType: MARKET_TYPE.MARKET,
    startAt: START_AT.QUESTION_CREATION_TIME,
    finalComparisonMeasurement: LAST_OBJECTIVE_MEASUREMENT,
  }) {
    const {
      agentPredictions,
      recentResult,
      allAggregations,
      measurable,
    } = await this._getTimeScoringData(agentId, measurableId);

    // Checks
    if (agentPredictions.length === 0) return undefined;
    if (allAggregations.length === 0) return undefined;
    if (
      params.finalComparisonMeasurement === LAST_OBJECTIVE_MEASUREMENT
      && !recentResult
    ) return undefined;
    if (!measurable) return undefined;

    // Use of Parameters
    const resolutionMeasurement =
      (params.finalComparisonMeasurement === LAST_OBJECTIVE_MEASUREMENT)
        ? recentResult
        : _.last(_.sortBy(allAggregations, (r) => r.relevantAt));

    if (!resolutionMeasurement) return undefined;

    const startTime = (params.startAt === START_AT.QUESTION_CREATION_TIME)
      ? measurable.createdAt
      : _.get(agentPredictions, '0.dataValues.relevantAt');

    const marketScoreType = params.marketType === MARKET_TYPE.MARKET
      ? marketScore
      : nonMarketScore;

    const scoreCalculator = this._scoreCalculator({
      agentPredictions,
      allAggregations,
      resolutionMeasurement,
      marketScoreType,
      startTime,
    });

    if (
      !scoreCalculator
      || !scoreCalculator.score
      || !scoreCalculator.distribution
    ) return undefined;

    const { score, distribution } = scoreCalculator;

    const _timeActivityRatio = timeActivityRatio({
      initialTime: measurable.createdAt,
      firstPredictionTime: _.get(agentPredictions, '0.dataValues.relevantAt'),
      endingTime: resolutionMeasurement.dataValues.relevantAt,
    });

    return {
      score: _.round(score, 6),
      agentPredictions,
      aggregations: allAggregations,
      recentResult,
      scoringStartTime: startTime,
      scoringEndTime: resolutionMeasurement.dataValues.relevantAt,
      measurableCreationTime: measurable.createdAt,
      finalResolutionTime: resolutionMeasurement.dataValues.relevantAt,
      timeActivityRatio: _.round(_timeActivityRatio, 6),
      activeTimeDistribution: {
        finalX: distribution.finalX,
        points: distribution.points,
      },
    };
  }

  /**
   * @param agentPredictions
   * @param allAggregations
   * @param resolutionMeasurement
   * @param marketScoreType
   * @param startTime
   * @returns {undefined|{score: *, distribution: *}}
   * @private
   */
  _scoreCalculator(
    {
      agentPredictions,
      allAggregations,
      resolutionMeasurement,
      marketScoreType,
      startTime,
    },
  ) {
    /**
     * @param v
     * @returns {{data: *, dataType: *}}
     */
    function translateValue(v) {
      let { data } = v;
      const { dataType } = v;
      if (dataType === 'percentage') {
        data /= 100;
      }
      return { data, dataType };
    }

    /**
     * @param p
     * @returns {{time: *, measurement: *}}
     */
    function toOverTime(p) {
      return {
        time: toUnix(p.dataValues.relevantAt),
        measurement: translateValue(p.dataValues.value),
      };
    }

    // Main Function
    if (!startTime) return undefined;

    let timeScore;
    let timeDistribution;

    try {
      timeScore = new PredictionResolutionOverTime({
        agentPredictions: agentPredictions.map(toOverTime),
        marketPredictions: allAggregations.map(toOverTime),
        resolution: toOverTime(resolutionMeasurement),
      }).averagePointScore(marketScoreType, toUnix(startTime));

      timeDistribution = new PredictionResolutionOverTime({
        agentPredictions: agentPredictions.map(toOverTime),
        marketPredictions: allAggregations.map(toOverTime),
        resolution: toOverTime(resolutionMeasurement),
      }).pointScoreDistribution(marketScoreType, toUnix(startTime));
    } catch (e) {
      log.trace(e.message);
      return undefined;
    }

    if (!!timeScore.error) {
      log.error('PrimaryPointScore Error: ', timeScore.error);
      return undefined;
    }

    if (!_.isFinite(timeScore.data)) {
      log.error(
        'Error: PrimaryPointScore score, '
        + `${timeScore.data} is not finite`,
      );
      return undefined;
    }

    return {
      score: timeScore.data,
      distribution: timeDistribution.data,
    };
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
   * @param {Models.MeasurableID} measurableId
   * @returns {Promise<*>}
   */
  async _getMeasurements(measurableId) {
    return this.measurements.getAll(new Filter({
      measurableId,
    }));
  }

  /**
   * @param {Models.MeasurableID} measurableId
   * @returns {Promise<*>}
   */
  async _getMeasurable(measurableId) {
    return this.measurables.getOne(new Params({
      id: measurableId,
    }));
  }
}

module.exports = {
  AgentMeasurablesData,
};
