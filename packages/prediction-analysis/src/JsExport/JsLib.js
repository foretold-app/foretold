const {
  JsResult_fromResult,
  MeasurementValueI_make,
  MeasurementValueI_differentialEntropy,
  PredictionResolutionGroupI_make,
  PredictionResolutionGroupI_pointScore,
  PredictionResolutionOverTimeI_make,
  PredictionResolutionOverTimeI_averagePointScore,
  PredictionResolutionOverTimeI_pointScoreDistribution,
  MarketType_marketScore,
  MarketType_nonMarketScore,
} = require('./JsInterface.gen');

const marketScore = MarketType_marketScore;
const nonMarketScore = MarketType_nonMarketScore;

/**
 * @param json
 * @constructor
 */
function MeasurementValue(json) {
  const make = JsResult_fromResult(MeasurementValueI_make(json));
  this.data = make.data;
  this.error = make.error;
}

/**
 * @returns {{data: *}}
 */
MeasurementValue.prototype.differentialEntropy = function () {
  return { data: MeasurementValueI_differentialEntropy(this.data) };
};

/**
 * @param agentPrediction
 * @param marketPrediction
 * @param resolution
 * @constructor
 */
function PredictionResolutionGroup(
  {
    agentPrediction,
    marketPrediction,
    resolution,
  }
) {
  const make = JsResult_fromResult(
    PredictionResolutionGroupI_make(
      agentPrediction,
      marketPrediction,
      resolution,
    )
  );
  this.data = make.data;
  this.error = make.error;
}

/**
 * @param marketScoreType
 * @returns {{error: string}|{data: *, error: *}|{error: *}}
 */
PredictionResolutionGroup.prototype.pointScore = function (marketScoreType) {
  if (marketScoreType !== marketScore && marketScoreType !== nonMarketScore) {
    return { error: 'Market score type invalid' };
  }
  if (!!this.error) {
    return { error: this.error };
  }
  return JsResult_fromResult(
    PredictionResolutionGroupI_pointScore(marketScoreType, this.data)
  );
};

/**
 * @param agentPredictions
 * @param marketPredictions
 * @param resolution
 * @constructor
 */
function PredictionResolutionOverTime(
  {
    agentPredictions,
    marketPredictions,
    resolution,
  }
) {
  const make = JsResult_fromResult(
    PredictionResolutionOverTimeI_make(
      agentPredictions,
      marketPredictions,
      resolution,
    )
  );
  this.data = make.data;
  this.error = make.error;
}

/**
 * @param marketScoreType
 * @param beginningTime
 * @returns {{error: string}|{data: *, error: *}|{error: *}}
 */
PredictionResolutionOverTime.prototype.averagePointScore = function (
  marketScoreType,
  beginningTime,
) {
  if (marketScoreType !== marketScore && marketScoreType !== nonMarketScore) {
    return { error: 'Market score type invalid' };
  }

  if (!!this.error) {
    return { error: this.error };
  }

  return JsResult_fromResult(
    PredictionResolutionOverTimeI_averagePointScore(
      marketScoreType,
      beginningTime,
      this.data,
    ),
  );
};

PredictionResolutionOverTime.prototype.pointScoreDistribution = function (
  marketScoreType,
) {
  if (marketScoreType !== marketScore && marketScoreType !== nonMarketScore) {
    return { error: 'Market score type invalid' };
  }

  if (!!this.error) {
    return { error: this.error };
  }

  return JsResult_fromResult(
    PredictionResolutionOverTimeI_pointScoreDistribution(
      marketScoreType,
      this.data,
    ),
  );
};

exports.MeasurementValue = MeasurementValue;
exports.PredictionResolutionGroup = PredictionResolutionGroup;
exports.PredictionResolutionOverTime = PredictionResolutionOverTime;
exports.marketScore = marketScore;
exports.nonMarketScore = nonMarketScore;
