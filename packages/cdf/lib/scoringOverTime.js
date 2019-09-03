
// class Prediction {
//   constructor(xs, ys) {
//   }
// }

// - predictions, aggregations, result.


// function predictionInputToPointValues({startTime, stopTime, predictions, aggregations, result}){
// }

// list of relevant timestamps
// For each timestamp, relevant score.

// function distsToPairing({predictions, aggregations}){

// }

function pointValueDistributionToTotalScore({startTime, stopTime, values}){
  let predictionLength = values.maxX() - values.minX();
  let fullLength = stopTime - startTime;
  let percentageOfFullLength = predictionLength / fullLength;
  return values.integral2() * percentageOfFullLength;
}

module.exports = {
  pointValueDistributionToTotalScore,
};
