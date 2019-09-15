
var {
    JsResult_fromResult,
    MeasurementValueI_make,
    MeasurementValueI_differentialEntropy,
    PredictionResolutionGroupI_make,
    PredictionResolutionGroupI_pointScore,
    PredictionResolutionOverTimeI_make,
    PredictionResolutionOverTimeI_averagePointScore,
    MarketType_marketScore,
    MarketType_nonMarketScore
} = require("./JsInterface.gen");

var marketScore = MarketType_marketScore;
var nonMarketScore = MarketType_nonMarketScore;

function MeasurementValue(json) {
    var make = JsResult_fromResult(MeasurementValueI_make(json));
    this.data = make.data;
    this.error = make.error;
};

MeasurementValue.prototype.differentialEntropy = function(){
    return {data: MeasurementValueI_differentialEntropy(this.data)}
};

function PredictionResolutionGroup({agentPrediction, marketPrediction, resolution}){
    var make = JsResult_fromResult(PredictionResolutionGroupI_make(agentPrediction, marketPrediction, resolution));
    this.data = make.data;
    this.error = make.error
};

PredictionResolutionGroup.prototype.pointScore = function(marketScoreType){
    if (marketScoreType !== marketScore && marketScoreType !== nonMarketScore){
        return {error: "Market score type invalid"}
    }
    if (!!this.error){ return { error: this.error } }
    return (
        JsResult_fromResult(PredictionResolutionGroupI_pointScore(marketScoreType, this.data))
    );
}

function PredictionResolutionOverTime({agentPredictions, marketPredictions, resolution}){
    var make = JsResult_fromResult(PredictionResolutionOverTimeI_make(agentPredictions, marketPredictions, resolution));
    this.data = make.data;
    this.error = make.error
};

PredictionResolutionOverTime.prototype.averagePointScore = function(marketScoreType, beginningTime){
    if (marketScoreType !== marketScore && marketScoreType !== nonMarketScore){
        return {error: "Market score type invalid"}
    }
    if (!!this.error){ return { error: this.error } }
    return JsResult_fromResult(
        PredictionResolutionOverTimeI_averagePointScore(marketScoreType, beginningTime, undefined, this.data)
    );
}

exports.MeasurementValue = MeasurementValue;
exports.PredictionResolutionGroup = PredictionResolutionGroup;
exports.PredictionResolutionOverTime = PredictionResolutionOverTime;
exports.marketScore = marketScore
exports.nonMarketScore = nonMarketScore