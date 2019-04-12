const measurables = require('./measurables');

describe('Measurables Authorizers', () => {
  it('returns as an object', () => {
    expect(measurables).toBeInstanceOf(Object);
  });
});
