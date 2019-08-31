const { distributionInputPointOutput, distributionInputDistributionOutput, percentageInputBinaryOutput } = require('./scoring');
const { Cdf } = require('./cdf');

describe('scoring', () => {
  it('distributionInputPointOutput()', () => {
    const predictionCdf = new Cdf([0,1,2,3,4,5], [0,.1,.2,.3,.4,1]);
    const aggregateCdf = new Cdf([0,1,2,3,4,5], [0,.4,.6,.7,.7,1]);
    let resultPoint = 4.5;
    let result = distributionInputPointOutput({predictionCdf, aggregateCdf, resultPoint});
    expect(result).toBeCloseTo(1.22);
  })
  it('distributionInputDistributionOutput()', () => {
    const predictionCdf = new Cdf([0,1,2,3,4,5], [0,.1,.2,.3,.4,1]);
    const aggregateCdf = new Cdf([0,1,2,3,4,5], [0,.4,.6,.7,.7,1]);
    const resultCdf = new Cdf([0,1,2,3,4,5], [0,0,0,0,1,1]);
    let result = distributionInputDistributionOutput({predictionCdf, aggregateCdf, resultCdf});
    expect(result).toBeCloseTo(2.006);
  })
  it('distributionInputDistributionOutput()', () => {
    const predictionCdf = new Cdf([0,100,200,300,400,500], [0,.1,.2,.3,.4,1]);
    const aggregateCdf = new Cdf([0,100,200,300,400,500], [0,.4,.6,.7,.7,1]);
    const resultCdf = new Cdf([0,100,200,300,400,500], [0,0,0,0,1,1]);
    let result = distributionInputDistributionOutput({predictionCdf, aggregateCdf, resultCdf});
    expect(result).toBeCloseTo(2.006);
  })
  it('distributionInputDistributionOutput()', () => {
    let result = percentageInputBinaryOutput({predictionPercentage:.3, aggregatePercentage:.8, resultIsSuccess:true});
    expect(result).toBeCloseTo(-1.415);
  })
  it('distributionInputDistributionOutput False()', () => {
    let result = percentageInputBinaryOutput({predictionPercentage:.3, aggregatePercentage:.8, resultIsSuccess:false});
    expect(result).toBeCloseTo(1.807);
  })
})