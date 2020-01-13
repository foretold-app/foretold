const index = require('./materialized-views-updater');

describe('Materialized Views Updater Async', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
