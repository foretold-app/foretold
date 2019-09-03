
// class Prediction {
//   constructor(xs, ys) {
//   }
// }

// - predictions, aggregations, result.


// function predictionInputToPointValues({startTime, stopTime, predictions, aggregations, result}){
// }

function distsToPairing({predictions, aggregations}){


}

function pointValueDistributionToTotalScore({startTime, stopTime, values, totalValue}){
  let predictionLength = values.maxX() - values.minX();
  let fullLength = stopTime - startTime;
  let percentageOfFullLength = predictionLength / fullLength;
  console.log("HI", percentageOfFullLength, totalValue)
  return values.integral2() * percentageOfFullLength;
}

module.exports = {
  pointValueDistributionToTotalScore,
};
