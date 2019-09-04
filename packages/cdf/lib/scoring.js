const { ContinuousDistributionCombination } = require('./continuousDistributionCombination');
const _ = require('lodash');

let scoreFunctionWithoutAggregateOrResult = ({prediction}) => Math.log2(prediction);
let scoreFunctionWithoutResult = ({prediction, aggregate}) => Math.log2(prediction) - Math.log2(aggregate);
let scoreFunctionWithResult = ({prediction, aggregate, result}) => scoreFunctionWithoutResult({prediction, aggregate}) * result;

let maxScore = 20;
let minScore = -20;

let ensureScoreInBounds = score => {
    if (score > maxScore){
        return maxScore
    } else if (score < minScore){
        return minScore
    } else if (!_.isFinite(score)){
        console.error(`Score is not valid: ${score}. Submitting as NaN.`);
        return NaN
    } else {
        return score
    }
}

function _distributionInputPointOutputSingle({predictionCdf, resultPoint}){
    let yAtPoint = predictionCdf.toPdf().findY(resultPoint);
    return scoreFunctionWithoutAggregateOrResult({prediction: yAtPoint});
}

function distributionInputPointOutput({predictionCdf, aggregateCdf, resultPoint}){
    let predictionError = _distributionInputPointOutputSingle({predictionCdf, resultPoint});
    let aggregateError = _distributionInputPointOutputSingle({predictionCdf: aggregateCdf, resultPoint});
    return ensureScoreInBounds(predictionError - aggregateError);
}

function distributionInputDistributionOutputDistribution({predictionCdf, aggregateCdf, resultCdf, sampleCount=10000}){
    let _predictionPdf = predictionCdf.toPdf();
    let _aggregatePdf = aggregateCdf.toPdf();
    let _resultPdf = resultCdf.toPdf();
    let combination = new ContinuousDistributionCombination([_predictionPdf, _aggregatePdf, _resultPdf]);
    let continuousResult = combination.combineYsWithFn(sampleCount,
        r => scoreFunctionWithResult({prediction: r[0], aggregate: r[1], result: r[2]})
    );
    return continuousResult;
}

function distributionInputDistributionOutput({predictionCdf, aggregateCdf, resultCdf, sampleCount=10000}){
    let pdfResult = distributionInputDistributionOutputDistribution({predictionCdf, aggregateCdf, resultCdf, sampleCount});
    return ensureScoreInBounds(pdfResult.integral());
}

function percentageInputPercentageOutput({predictionPercentage, aggregatePercentage, resultPercentage}){
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

let scoringFunctions = {
  distributionInputPointOutput,
  distributionInputDistributionOutputDistribution,
  distributionInputDistributionOutput,
  percentageInputPercentageOutput,
  scoreFunctionWithoutResult,
  scoreFunctionWithResult
};

module.exports = {
  scoringFunctions
};