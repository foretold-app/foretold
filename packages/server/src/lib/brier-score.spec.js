const { BrierScore } = require('./brier-score');

describe('BrierScore', () => {
  it('returns an object', () => {
    expect(BrierScore).toBeInstanceOf(Function);
  });
  it('for true results', () => {
    const data = [30, 40, 50, 70, 80, 100];
    const result = true;
    expect(new BrierScore(data, result).brierScores()).toEqual([
      0.49, 0.36, 0.25, 0.09, 0.04, 0,
    ]);
  });
  it('for false results', () => {
    const data = [30, 40, 50, 70, 80, 100];
    const result = false;
    expect(new BrierScore(data, result).brierScores()).toEqual([
      0.09, 0.16, 0.25, 0.49, 0.64, 1,
    ]);
  });
});
