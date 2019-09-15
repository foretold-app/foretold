
var {
    JsResult_fromResult,
    MeasurementValueI_make,
    PredictionResolutionGroupI_make,
    PredictionResolutionGroupI_pointScore,
    PredictionResolutionOverTimeI_make,
    PredictionResolutionOverTimeI_pointScoreIntegral
} = require("./JsInterface.gen");

function MeasurementValue(json) {
    var make = MeasurementValueI_make(json);
    this.data = make.data;
    this.error = make.error
    return {data: this, error: this.error}
};

function PredictionResolutionGroup({agentPrediction, marketPrediction, resolution}){
    var make = JsResult_fromResult(PredictionResolutionGroupI_make(agentPrediction, marketPrediction, resolution));
    this.data = make.data;
    this.error = make.error
    return {data: this, error: this.error}
};

PredictionResolutionGroup.prototype.pointScore = function(){
    if (!this.error){
        return { error: this.error }
    } else {
        return JsResult_fromResult(
            PredictionResolutionGroupI_pointScore(this.data)
        );
    }
}

function PredictionResolutionOverTime({agentPredictions, marketPredictions, resolution}){
    var make = JsResult_fromResult(PredictionResolutionOverTimeI_make(agentPrediction, marketPrediction, resolution));
    this.data = make.data;
    this.error = make.error
    return {data: this, error: this.error}
};

PredictionResolutionGroup.prototype.pointScoreIntegral = function(){
    if (!this.error){
        return { error: this.error }
    } else {
        return JsResult_fromResult(
            PredictionResolutionOverTimeI_pointScoreIntegral(this.data)
        );
    }
}

exports.MeasurementValue = MeasurementValue;
exports.PredictionResolutionGroup = PredictionResolutionGroup;
exports.PredictionResolutionOverTime = PredictionResolutionOverTime;