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
    );
    return pdfResult.integral();
}

function percentageInputBinaryOutput({predictionPercentage, aggregatePercentage, resultIsSuccess=true}){
    let switchIfFalse = (e) => (resultIsSuccess == true) ? e : (1 - e);
    return genericScoringFunction({
        prediction: switchIfFalse(predictionPercentage),
        aggregate: switchIfFalse(aggregatePercentage)
    });
}

module.exports = {
  distributionInputPointOutput,
  distributionInputDistributionOutput,
  percentageInputBinaryOutput
};