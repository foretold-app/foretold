/* Untyped file generated from JsInterface.re by genType. */

const $$toRE824073253 = {"MarketScore": -870794282, "NonMarketScore": 463448073};

const CreateBucklescriptBlock = require('bs-platform/lib/js/block.js');

const Curry = require('bs-platform/lib/js/curry.js');

const JsInterfaceBS = require('./JsInterface.bs');

const JsResult_fromResult = function (Arg1) {
  const result = JsInterfaceBS.JsResult.fromResult(Arg1.tag==="Ok"
    ? CreateBucklescriptBlock.__(0, [Arg1.value])
    : CreateBucklescriptBlock.__(1, [Arg1.value]));
  return result
};;
exports.JsResult_fromResult = JsResult_fromResult

const MarketType_marketScore = "MarketScore";;
exports.MarketType_marketScore = MarketType_marketScore

const MarketType_nonMarketScore = "NonMarketScore";;
exports.MarketType_nonMarketScore = MarketType_nonMarketScore

const MeasurementValueI_make = function (Arg1) {
  const result = JsInterfaceBS.MeasurementValueI.make(Arg1);
  return result.tag===0
    ? {tag:"Ok", value:result[0]}
    : {tag:"Error", value:result[0]}
};;
exports.MeasurementValueI_make = MeasurementValueI_make

const MeasurementValueI_differentialEntropy = JsInterfaceBS.MeasurementValueI.differentialEntropy;;
exports.MeasurementValueI_differentialEntropy = MeasurementValueI_differentialEntropy

const PredictionResolutionGroupI_make = function (Arg1, Arg2, Arg3) {
  const result = Curry._3(JsInterfaceBS.PredictionResolutionGroupI.make, Arg1, (Arg2 == null ? undefined : Arg2), Arg3);
  return result.tag===0
    ? {tag:"Ok", value:result[0]}
    : {tag:"Error", value:result[0]}
};;
exports.PredictionResolutionGroupI_make = PredictionResolutionGroupI_make

const PredictionResolutionGroupI_pointScore = function (Arg1, Arg2) {
  const result = Curry._2(JsInterfaceBS.PredictionResolutionGroupI.pointScore, $$toRE824073253[Arg1], Arg2);
  return result.tag===0
    ? {tag:"Ok", value:result[0]}
    : {tag:"Error", value:result[0]}
};;
exports.PredictionResolutionGroupI_pointScore = PredictionResolutionGroupI_pointScore

const PredictionResolutionOverTimeI_make = function (Arg1, Arg2, Arg3) {
  const result = Curry._3(JsInterfaceBS.PredictionResolutionOverTimeI.make, Arg1, Arg2, Arg3);
  return result.tag===0
    ? {tag:"Ok", value:result[0]}
    : {tag:"Error", value:result[0]}
};;
exports.PredictionResolutionOverTimeI_make = PredictionResolutionOverTimeI_make

const PredictionResolutionOverTimeI_averagePointScore = function (Arg1, Arg2, Arg3) {
  const result = Curry._3(JsInterfaceBS.PredictionResolutionOverTimeI.averagePointScore, $$toRE824073253[Arg1], Arg2, Arg3);
  return result.tag===0
    ? {tag:"Ok", value:result[0]}
    : {tag:"Error", value:result[0]}
};;
exports.PredictionResolutionOverTimeI_averagePointScore = PredictionResolutionOverTimeI_averagePointScore

const PredictionResolutionOverTimeI_pointScoreDistribution = function (Arg1, Arg2) {
  const result = Curry._2(JsInterfaceBS.PredictionResolutionOverTimeI.pointScoreDistribution, $$toRE824073253[Arg1], Arg2);
  return result.tag===0
    ? {tag:"Ok", value:result[0]}
    : {tag:"Error", value:result[0]}
};;
exports.PredictionResolutionOverTimeI_pointScoreDistribution = PredictionResolutionOverTimeI_pointScoreDistribution

const PredictionResolutionOverTimeI = JsInterfaceBS.PredictionResolutionOverTimeI;
exports.PredictionResolutionOverTimeI = PredictionResolutionOverTimeI

const MeasurementValueI = JsInterfaceBS.MeasurementValueI;
exports.MeasurementValueI = MeasurementValueI

const MarketType = JsInterfaceBS.MarketType;
exports.MarketType = MarketType

const PredictionResolutionGroupI = JsInterfaceBS.PredictionResolutionGroupI;
exports.PredictionResolutionGroupI = PredictionResolutionGroupI

const JsResult = JsInterfaceBS.JsResult;
exports.JsResult = JsResult
