// const def = x => typeof x !== 'undefined'
// const reverse = ([x, ...xs]) => def(x) ? [...reverse(xs), x] : []
// const flow = (...args) => init => reduce(args, (memo, fn) => fn(memo), init);
// const compose = (...args) => flow(...reverse(args))

import { MEASUREMENT_TYPE, SCORING_MARKET_TYPE, SCORING_PAIR } from "../enums"

const scoringRules = {
    logScore: {
        individualLossFn: ({primary}) => Math.log2(primary),
        relativeLossFn: ({primary, comparison}) => Math.log2(primary) - Math.log2(comparison),
        individualLossScoreFn: ({result, lossScore}) => result * lossScore
    }
}

let cdfToResultY = (cdf, resultPoint) => cdf.toPdf().findY(resultPoint);

let inverse = (e) => (1 - e);

let pairToScoringPair = (predictionType, resultType) => {
    if (predictionType === MEASUREMENT_TYPE.floatCdf && resultType === MEASUREMENT_TYPE.floatPoint){
        return SCORING_PAIR.FLOAT_CDF_PREDICTION_FLOAT_POINT_RESULT
    } else if (predictionType === MEASUREMENT_TYPE.floatCdf && resultType === MEASUREMENT_TYPE.floatCdf){
        return SCORING_PAIR.FLOAT_CDF_PREDICTION_FLOAT_CDF_RESULT
    } else if (predictionType === MEASUREMENT_TYPE.percentage && resultType === MEASUREMENT_TYPE.percentage){
        return SCORING_PAIR.PERCENTAGE_PREDICTION_PERCENTAGE_RESULT
    } else {
        return SCORING_PAIR.INVALID
    }
}

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
    
    

    // let scoringPair = pairToScoringPair(predictionMeasurementType, resultMeasurementType);
    // if (predictionMeasurementType === MEASUREMENT_TYPE.floatCdf && resultMeasurementType === MEASUREMENT_TYPE.floatCdf && scoringMarketType === SCORING_MARKET_TYPE.NON_MARKET){

    // }
    // if (scoringPair === SCORING_PAIR.FLOAT_CDF_PREDICTION_FLOAT_POINT_RESULT && scoringMarketType === SCORING_MARKET_TYPE.NON_MARKET){
        
    // } else if (scoringPair === SCORING_PAIR.FLOAT_CDF_PREDICTION_FLOAT_POINT_RESULT && scoringMarketType === SCORING_MARKET_TYPE.MARKET){
    //     return costFn(cdfToResultY(scoringPrediction, resultPoint) / cdfToResultY(marketPrediction, resultPoint))
    // } else if (scoringPair === SCORING_PAIR.FLOAT_CDF_PREDICTION_FLOAT_CDF_RESULT && scoringMarketType === SCORING_MARKET_TYPE.NON_MARKET){
    //     let combination = new ContinuousDistributionCombination([scoringPrediction.toPdf(), result.toPdf()]);
    //     let distributionScore = combination.combineYsWithFn(sampleCount,
    //         r => r[1] * costFn(r[0])
    //     );
    //     return distributionScore.integral();
    // } else if (scoringPair === SCORING_PAIR.FLOAT_CDF_PREDICTION_FLOAT_CDF_RESULT && scoringMarketType === SCORING_MARKET_TYPE.MARKET){
    //     let combination = new ContinuousDistributionCombination([scoringPrediction.toPdf(), marketPrediction.toPdf(), result.toPdf()]);
    //     let distributionScore = combination.combineYsWithFn(sampleCount,
    //         r => r[2] * (costFn(r[0] / r[1]))
    //     );
    //     return distributionScore.integral();
    // } else if (scoringPair === SCORING_PAIR.PERCENTAGE_PREDICTION_PERCENTAGE_RESULT && scoringMarketType === SCORING_MARKET_TYPE.NON_MARKET){
    //     return (result * costFn(scoringPrediction)) + (inverse(result) * costFn(inverse(scoringPrediction)));
    // } else if (scoringPair === SCORING_PAIR.PERCENTAGE_PREDICTION_PERCENTAGE_RESULT && scoringMarketType === SCORING_MARKET_TYPE.MARKET){
    //     return (result * costFn(scoringPrediction/marketPrediction)) + (inverse(result) * costFn(inverse(scoringPrediction)/inverse(marketPrediction)));
    // }


// different choices
// distribution -> point
// distribution -> distribution
// percentage -> percentage

class IndividualScore {
  constructor(distributions) {
    this.distributions = distributions;
  }
}

class RelativeScore {
  constructor(distributions) {
    this.distributions = distributions;
  }
}