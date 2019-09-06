const schema = require('./schema');

describe('Schema', () => {
  it('returns as an object', () => {
    expect(schema).toBeInstanceOf(Object);
  });
});
