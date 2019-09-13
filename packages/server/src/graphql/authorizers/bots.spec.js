const bots = require('./bots');

describe('Bots Authorizers', () => {
  it('returns bots as an object', () => {
    expect(bots).toBeInstanceOf(Object);
  });
});
