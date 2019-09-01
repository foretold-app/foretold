const { ContinuousDistributionCombination } = require('./continuousDistributionCombination');

let scoreFunctionWithoutResult = ({prediction, aggregate}) => Math.log2(prediction / aggregate);
let scoreFunctionWithResult = ({prediction, aggregate, result}) => scoreFunctionWithoutResult({prediction, aggregate}) * result;

function distributionInputPointOutput({predictionCdf, aggregateCdf, resultPoint}){
    let _predictionPdfValue = (predictionCdf).toPdf().findY(resultPoint);
    let _aggregatePdfValue = (aggregateCdf).toPdf().findY(resultPoint);
    return genericScoringFunction({prediction: _predictionPdfValue, aggregate: _aggregatePdfValue});
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
    return pdfResult.integral();
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
    return isFalseFactor + isTrueFactor;
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