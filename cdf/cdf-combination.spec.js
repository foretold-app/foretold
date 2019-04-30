const { Cdf } = require('./cdf');
const { CdfCombination } = require('./cdf-combination');
const { up, down } = require('./functions');

describe('CdfCombination Class', () => {
  it('minBounds()', () => {
    const cdfs = [
      new Cdf(up(1, 9), down(9, 1)),
      new Cdf(up(20, 30), down(60, 50)),
    ];
    const comb = new CdfCombination(cdfs);
    expect(comb.minBounds()).toBe(1);
  });
  it('maxBounds()', () => {
    const cdfs = [
      new Cdf(up(1, 9), down(9, 1)),
      new Cdf(up(20, 30), down(60, 50)),
    ];
    const comb = new CdfCombination(cdfs);
    expect(comb.maxBounds()).toBe(20);
  });
  it('xsToAnalyze()', () => {
    const cdfs = [
      new Cdf(up(1, 9), down(9, 1)),
      new Cdf(up(20, 30), down(60, 50)),
    ];
    const comb = new CdfCombination(cdfs);
    expect(comb.xsToAnalyze(3)).toEqual([1, 10.5, 20]);
  });
  it('allYsAtXPoint()', () => {
    const cdfs = [
      new Cdf(up(1, 9), down(9, 1)),
      new Cdf(up(1, 9), up(1, 9)),
      new Cdf([1, 5], [7, 8]),
      new Cdf(up(1, 9), down(60, 52)),
    ];
    const comb = new CdfCombination(cdfs);
    expect(comb.allYsAtXPoint(5)).toEqual([5, 5, 8, 56]);
  });
  it('allYsAtXPoint()', () => {
    const cdfs = [
      new Cdf(up(1, 9), down(9, 1)),
      new Cdf(up(1, 9), up(1, 9)),
      new Cdf([1, 5], [7, 8]),
      new Cdf(up(1, 9), down(60, 52)),
    ];
    const comb = new CdfCombination(cdfs);
    expect(comb.meanOfYsAtXPoint(5)).toBe(18.5);
  });
  it('combine()', () => {
    const cdfs = [
      new Cdf(up(1, 9), up(1, 9)),
      new Cdf(up(1, 9), up(1, 9)),
    ];
    const comb = new CdfCombination(cdfs);
    const combined = comb.combine(5);
    expect(combined.xs).toEqual(true);
  });
});
