const bots = require('./bots');

describe('Bots Resolvers', () => {
  it('should be object', () => {
    expect(bots).toBeInstanceOf(Object);
  });
});
