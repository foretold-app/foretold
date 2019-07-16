const { Consumer } = require('./consumer');

describe('Consumer', () => {
  it('returns an constructor', () => {
    expect(Consumer).toBeInstanceOf(Function);
  });
});
