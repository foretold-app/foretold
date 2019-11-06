const { scoringFunctions: {distributionInputPointOutput, distributionInputDistributionOutput, percentageInputPercentageOutput} } = require('./scoring');
const { Cdf } = require('./cdf');

describe('scoring', () => {
  it('distributionInputDistributionOutput()', () => {
    const predictionCdf = new Cdf([0,1,2,3,4,5], [0,.1,.2,.3,.4,1]);
    const aggregateCdf = new Cdf([0,1,2,3,4,5], [0,.4,.6,.7,.7,1]);
    const resultCdf = new Cdf([0,1,2,3,4,5], [0,1.,.2, .3,1,1]);
    let result = distributionInputDistributionOutput({predictionCdf, aggregateCdf, resultCdf, sampleCount:5});
    expect(result.ys).toStrictEqual([NaN, -0.9940452071316824, 0.20473687525240464, 1.1, 0]);
  })
  it('distributionInputDistributionOutput()', () => {
    const predictionCdf = new Cdf([0,100,200,300,400,500], [0,.1,.2,.3,.4,1]);
    const aggregateCdf = new Cdf([0,100,200,300,400,500], [0,.4,.6,.7,.7,1]);
    const resultCdf = new Cdf([0,100,200,300,400,500], [0,0,0,0,1,1]);
    let result = distributionInputDistributionOutput({predictionCdf, aggregateCdf, resultCdf, sampleCount:5});
    expect(result.ys).toStrictEqual([NaN, -0, -0, 0.015000000000000003, 0]);
  })
  it('distributionInputDistributionOutput()', () => {
    const predictionCdf = new Cdf([0,100,200,300,400,500], [0,0,0,.1,.1,1]);
    const aggregateCdf = new Cdf([0,100,200,300,400,500], [0,.4,.6,.7,.7,1]);
    const resultCdf = new Cdf([0,100,200,300,400,500], [0,.1, .2, .3, .6, 1.0]);
    let result = distributionInputDistributionOutput({predictionCdf, aggregateCdf, resultCdf, sampleCount:5});
    expect(result.ys).toStrictEqual([NaN, -Infinity, -0.0015849625007211558, 8.008566259537293e-19,0.006339850002884624]);
  })
})