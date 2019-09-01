const { Cdf } = require('./cdf');
const { ContinuousDistributionCombination } = require('./continuousDistributionCombination');

let genericScoringFunction = ({prediction, aggregate}) => Math.log2(prediction / aggregate);

function distributionInputPointOutput({predictionCdf, aggregateCdf, resultPoint}){
    let _predictionPdfValue = (predictionCdf).toPdf().findY(resultPoint);
    let _aggregatePdfValue = (aggregateCdf).toPdf().findY(resultPoint);
    return genericScoringFunction({prediction: _predictionPdfValue, aggregate: _aggregatePdfValue});
}

function distributionInputDistributionOutput({predictionCdf, aggregateCdf, resultCdf, sampleCount=10000}){
    let _predictionPdf = predictionCdf.toPdf();
    let _aggregatePdf = aggregateCdf.toPdf();
    let _resultPdf = resultCdf.toPdf();
    let combination = new ContinuousDistributionCombination([_predictionPdf, _aggregatePdf, _resultPdf]);
    let pdfResult = combination.combineYsWithFn(sampleCount,
        r => genericScoringFunction({prediction: r[0], aggregate: r[1]}) * r[2]
    )
    return pdfResult.integral();
}

function percentageInputPercentageOutput({predictionPercentage, aggregatePercentage, resultPercentage}){
    let inverse = (e) => (1 - e);
    let isFalseFactor = resultPercentage * genericScoringFunction({prediction: predictionPercentage, aggregate:aggregatePercentage});
    let isTrueFactor = inverse(resultPercentage) * genericScoringFunction({prediction: inverse(predictionPercentage), aggregate:inverse(aggregatePercentage)});
    return isFalseFactor + isTrueFactor;
}

module.exports = {
  distributionInputPointOutput,
  distributionInputDistributionOutput,
  percentageInputPercentageOutput
};