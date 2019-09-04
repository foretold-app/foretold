import { MEASUREMENT_TYPE, SCORING_MARKET_TYPE, SCORING_PAIR } from "../enums"

let cdfToResultY = (cdf, resultPoint) => cdf.toPdf().findY(resultPoint);

let inverse = (e) => (1 - e);

function scoreThisThing({
    scoreType: {
        predictionMeasurementType=MEASUREMENT_TYPE.floatCdf,
        resultMeasurementType=MEASUREMENT_TYPE.floatPoint,
        scoringMarketType=SCORING_MARKET_TYPE.market,
    },
    inputs: {
        scoringPrediction={},
        marketPrediction={},
        result={},
    },
    config: {
        costFn=Math.log2,
        sampleCount=10000
    }
}){
    switch([predictionMeasurementType, resultMeasurementType, scoringMarketType].join(",")){
        case [MEASUREMENT_TYPE.floatCdf, MEASUREMENT_TYPE.floatPoint, SCORING_MARKET_TYPE.NON_MARKET].join(","):
            return costFn(cdfToResultY(scoringPrediction, resultPoint))
            break
        case [MEASUREMENT_TYPE.floatCdf, MEASUREMENT_TYPE.floatPoint, SCORING_MARKET_TYPE.MARKET].join(","):
            return costFn(cdfToResultY(scoringPrediction, resultPoint) / cdfToResultY(marketPrediction, resultPoint))
            break
        case [MEASUREMENT_TYPE.floatCdf, MEASUREMENT_TYPE.floatCdf, SCORING_MARKET_TYPE.NON_MARKET].join(","):
            let combination = new ContinuousDistributionCombination([scoringPrediction.toPdf(), result.toPdf()]);
            let distributionScore = combination.combineYsWithFn(sampleCount,
                r => r[1] * costFn(r[0])
            );
            return distributionScore.integral();
            break
        case [MEASUREMENT_TYPE.floatCdf, MEASUREMENT_TYPE.floatCdf, SCORING_MARKET_TYPE.MARKET].join(","):
            let combination = new ContinuousDistributionCombination([scoringPrediction.toPdf(), marketPrediction.toPdf(), result.toPdf()]);
            let distributionScore = combination.combineYsWithFn(sampleCount,
                r => r[2] * (costFn(r[0] / r[1]))
            );
            return distributionScore.integral();
            break
        case [MEASUREMENT_TYPE.percentage, MEASUREMENT_TYPE.percentage, SCORING_MARKET_TYPE.NON_MARKET].join(","):
            return (result * costFn(scoringPrediction)) + (inverse(result) * costFn(inverse(scoringPrediction)));
            break
        case [MEASUREMENT_TYPE.percentage, MEASUREMENT_TYPE.percentage, SCORING_MARKET_TYPE.MARKET].join(","):
            return (result * costFn(scoringPrediction/marketPrediction)) + (inverse(result) * costFn(inverse(scoringPrediction)/inverse(marketPrediction)));
            break
    }
}