// Third-part dependencies
const _ = require('lodash');
const moment = require('moment');
const {
  PredictionResolutionOverTime,
} = require('@foretold/prediction-analysis');
const { marketScore } = require('@foretold/prediction-analysis');
const { nonMarketScore } = require('@foretold/prediction-analysis');

// The first level of dependencies (..)
const logger = require('../../lib/log');

const { MARKET_TYPE } = require('../../enums');
const { START_AT } = require('../../enums');
const { FINAL_COMPARISON_MEASUREMENT } = require('../../enums');

// The second level (.)
const log = logger.module('data/scoring/proceed');

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
    time: toUnix(p.relevantAt),
    measurement: translateValue(p.value),
  };
}


class Proceed {
  /**
   * Do not make any optimization here, it is early for this.
   * For each optimization we need to do a researching of the performance.
   */
  primaryPointScore2(
    {
      agentPredictions,
      recentResult,
      allAggregations,
      measurable,
    }, params = {
      marketType: MARKET_TYPE.MARKET,
      startAt: START_AT.QUESTION_CREATION_TIME,
      finalComparisonMeasurement: LAST_OBJECTIVE_MEASUREMENT,
    },
  ) {
    // Checks
    if (_.size(agentPredictions) === 0) return undefined;
    if (_.size(allAggregations) === 0) return undefined;
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
      : _.get(agentPredictions, '0.relevantAt', null);

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
      firstPredictionTime: _.get(agentPredictions, '0.relevantAt', null),
      endingTime: resolutionMeasurement.relevantAt,
    });

    return {
      score: _.round(score, 6),
      agentPredictions,
      aggregations: allAggregations,
      recentResult,
      scoringStartTime: startTime,
      scoringEndTime: resolutionMeasurement.relevantAt,
      measurableCreationTime: measurable.createdAt,
      finalResolutionTime: resolutionMeasurement.relevantAt,
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
   * @protected
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
}

module.exports = {
  Proceed,
};
