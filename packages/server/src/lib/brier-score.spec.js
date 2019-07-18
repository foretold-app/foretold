const { BrierScore } = require('./brier-score');

describe('BrierScore', () => {
  it('returns an object', () => {
    expect(BrierScore).toBeInstanceOf(Function);
  });
});
