const {
  ContinuousDistributionCombination,
} = require('./continuousDistributionCombination');
const _ = require('lodash');

let scoreFunctionWithoutResult = ({ prediction, aggregate }) => Math.log2(prediction / aggregate);

let scoreFunctionWithResult = ({ prediction, aggregate, result }) => scoreFunctionWithoutResult({
  prediction,
  aggregate
}) * result;

/**
 * @param predictionCdf
 * @param aggregateCdf
 * @param resultCdf
 * @param sampleCount
 * @returns {Cdf}
 */
function distributionInputDistributionOutput(
  { predictionCdf, aggregateCdf, resultCdf, sampleCount = 10000 },
) {
  let _predictionPdf = predictionCdf.toPdf();
  let _aggregatePdf = aggregateCdf.toPdf();
  let _resultPdf = resultCdf.toPdf();

  let combination = new ContinuousDistributionCombination([
    _predictionPdf, _aggregatePdf, _resultPdf,
  ]);

  return combination.combineYsWithFn(sampleCount,
    r =>
    scoreFunctionWithResult({
      prediction: r[0],
      aggregate: r[1],
      result: r[2]
    })
  );
}

/**
 * @param predictionCdf
 * @param resultCdf
 * @param sampleCount
 * @returns {Cdf}
 */
function distributionInputDistributionOutputMarketless(
  {
    predictionCdf,
    resultCdf,
    sampleCount = 10000
  }
) {
  let _predictionPdf = predictionCdf.toPdf();
  let _resultPdf = resultCdf.toPdf();

  let combination = new ContinuousDistributionCombination([
    _predictionPdf, _resultPdf,
  ]);

  return combination.combineYsWithFn(sampleCount,
    r => {
      if (r[1] === 0){
        return 0;
      } else {
        return (r[1] * Math.log2(Math.abs(r[0] / r[1])));
      }
    }
  );
}

/**
 * @param cdf
 * @param sampleCount
 * @returns {Cdf}
 */
function differentialEntropy({ cdf, sampleCount = 10000 }) {
  let _cdf = (cdf).toPdf();
  let combination = new ContinuousDistributionCombination([_cdf]);
  return combination.combineYsWithFn(sampleCount,
    r => r[0] * Math.log2(r[0])
  );
}

const scoringFunctions = {
  distributionInputDistributionOutputMarketless,
  distributionInputDistributionOutput,
  scoreFunctionWithoutResult,
  scoreFunctionWithResult,
  differentialEntropy,
};

module.exports = {
  scoringFunctions,
};