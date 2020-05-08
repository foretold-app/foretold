const index = require('./well-known');

describe('WellKnown Configuration', () => {
  it('returns an object', () => {
    expect(!!index).toBe(true);
  });
});
