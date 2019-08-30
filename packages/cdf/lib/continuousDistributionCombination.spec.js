const { Cdf } = require('./cdf');
const { ContinuousDistributionCombination } = require('./continuousDistributionCombination');
const { up } = require('./functions');

describe('ContinuousDistributionCombination Class', () => {
  it('minX()', () => {
    const cdfs = [
      new Cdf(up(1, 9), up(1, 9)),
      new Cdf(up(20, 30), up(50, 60)),
    ];
    const comb = new ContinuousDistributionCombination(cdfs);
    expect(comb.minX()).toBe(1);
  });
  it('maxBounds()', () => {
    const cdfs = [
      new Cdf(up(1, 9), up(1, 9)),
      new Cdf(up(20, 30), up(50, 60)),
    ];
    const comb = new ContinuousDistributionCombination(cdfs);
    expect(comb.maxX()).toBe(30);
  });
  it('equallyDividedXs()', () => {
    const cdfs = [
      new Cdf(up(1, 9), up(1, 9)),
      new Cdf(up(20, 30), up(50, 60)),
    ];
    const comb = new ContinuousDistributionCombination(cdfs);
    expect(comb.equallyDividedXs(3)).toEqual([1, 15.5, 30]);
  });
  it('allYsAtXPoint()', () => {
    const cdfs = [
      new Cdf(up(1, 9), up(1, 9)),
      new Cdf([1, 5], [7, 8]),
      new Cdf(up(1, 9), up(52, 60)),
    ];
    const comb = new ContinuousDistributionCombination(cdfs);
    expect(comb.allYsAtXPoint(5)).toEqual([5, 8, 56]);
  });
  it('combineYsWithMean()', () => {
    const cdfs = [
      new Cdf(up(1, 9), up(1, 9)),
      new Cdf(up(21, 29), up(11, 19)),
    ];
    const comb = new ContinuousDistributionCombination(cdfs);
    const combined = comb.combineYsWithMean(5);
    expect(combined.xs).toEqual([1, 8, 15, 22, 29]);
    expect(combined.ys).toEqual([6, 9.5, 10, 10.5, 14]);
  });
});
