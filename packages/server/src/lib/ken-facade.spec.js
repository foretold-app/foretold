const { KenFacade } = require('./ken-facade');

describe('Ken Facade', () => {
  it('returns an object', () => {
    expect(KenFacade).toBeInstanceOf(Function);
  });
});
