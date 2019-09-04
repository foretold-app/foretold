const { ContinuousDistribution } = require('./continuousDistribution');
const { pointValueDistributionToTotalScore } = require('./scoringOverTime');

describe('scoringOverTime', () => {
  it('pointValuesToTotalScore()', () => {
    const xs = [5,10];
    const ys = [1,1];
    let values = new ContinuousDistribution(xs, ys)
    let totalScore = pointValueDistributionToTotalScore({startTime: 0, stopTime: 10, values});
    expect(totalScore).toEqual(2.5);
  })

  it('pointValuesToTotalScore2()', () => {
    const xs = [1,5,9];
    const ys = [2,2,-2];
    let values = new ContinuousDistribution(xs, ys);
    let totalScore = pointValueDistributionToTotalScore({startTime: 0, stopTime: 10, values});
    expect(totalScore).toEqual(12.8);
  })
})