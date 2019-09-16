const {
  ContinuousDistributionCombination,
} = require('./continuousDistributionCombination');
const _ = require('lodash');

let scoreFunctionWithoutResult = ({ prediction, aggregate }) => Math.log2(prediction / aggregate);

let scoreFunctionWithResult = ({ prediction, aggregate, result }) => scoreFunctionWithoutResult({
  prediction,
  aggregate
}) * result;

let maxScore = 20;
let minScore = -20;

/**
 * @param score
 * @returns {number|*}
 */
let ensureScoreInBounds = score => {
  if (score === Infinity || (score > maxScore)) {
    return maxScore
  } else if (score === -Infinity || (score < minScore)) {
    return minScore
  } else if (!_.isFinite(score)) {
    return 0
  } else {
    return score
  }
};

/**
 * @param predictionCdf
 * @param aggregateCdf
 * @param resultPoint
 * @returns {number|*}
 */
function distributionInputPointOutput(
  { predictionCdf, aggregateCdf, resultPoint },
) {

  let _predictionPdfValue = (predictionCdf).toPdf().findY(resultPoint);
  let _aggregatePdfValue = (aggregateCdf).toPdf().findY(resultPoint);

  return ensureScoreInBounds(scoreFunctionWithoutResult({
    prediction: _predictionPdfValue,
    aggregate: _aggregatePdfValue
  }));
}

/**
 * @param predictionCdf
 * @param resultPoint
 * @returns {number}
 */
function distributionInputPointOutputMarketless(
  { predictionCdf, resultPoint },
) {
  let _predictionPdfValue = (predictionCdf).toPdf().findY(resultPoint);
  return Math.log2(_predictionPdfValue);
}

/**
 * @param predictionCdf
 * @param aggregateCdf
 * @param resultCdf
 * @param sampleCount
 * @returns {Cdf}
 */
function distributionInputDistributionOutputDistribution(
  { predictionCdf, aggregateCdf, resultCdf, sampleCount = 10000 },
) {

  let _predictionPdf = predictionCdf.toPdf();
  let _aggregatePdf = aggregateCdf.toPdf();
  let _resultPdf = resultCdf.toPdf();

  let combination = new ContinuousDistributionCombination([
    _predictionPdf, _aggregatePdf, _resultPdf,
  ]);

  return combination.combineYsWithFn(sampleCount,
    r => scoreFunctionWithResult({
      prediction: r[0],
      aggregate: r[1],
      result: r[2]
    })
  );
}

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
  return distributionInputDistributionOutputDistribution({
    predictionCdf,
    aggregateCdf,
    resultCdf,
    sampleCount
  });
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
      return (r[1] * Math.log2(Math.abs(r[0] / r[1])));
    }
  );
}

/**
 * @param predictionPercentage
 * @param aggregatePercentage
 * @param resultPercentage
 * @returns {number|*}
 */
function percentageInputPercentageOutput(
  { predictionPercentage, aggregatePercentage, resultPercentage },
) {
  let inverse = (e) => (1 - e);
  let isFalseFactor = scoreFunctionWithResult({
    prediction: predictionPercentage,
    aggregate: aggregatePercentage,
    result: resultPercentage
  });
  let isTrueFactor = scoreFunctionWithResult({
    prediction: inverse(predictionPercentage),
    aggregate: inverse(aggregatePercentage),
    result: inverse(resultPercentage)
  });

  return ensureScoreInBounds(isFalseFactor + isTrueFactor);
}

/**
 * @param predictionPercentage
 * @param resultPercentage
 * @returns {number}
 */
function percentageInputPercentageOutputMarketless(
  { predictionPercentage, resultPercentage },
) {
  let inverse = (e) => (1 - e);
  let isFalseFactor = resultPercentage * Math.log2(predictionPercentage);
  let isTrueFactor = inverse(resultPercentage) * Math.log2(inverse(predictionPercentage));
  return isFalseFactor + isTrueFactor;
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
  distributionInputPointOutputMarketless,
  distributionInputDistributionOutputMarketless,
  percentageInputPercentageOutputMarketless,
  distributionInputPointOutput,
  distributionInputDistributionOutputDistribution,
  distributionInputDistributionOutput,
  percentageInputPercentageOutput,
  scoreFunctionWithoutResult,
  scoreFunctionWithResult,
  differentialEntropy,
};

module.exports = {
  scoringFunctions,
};