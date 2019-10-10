const { initContext } = require('./init-context');

describe('Init Context', () => {
  it('returns a construction', () => {
    expect(initContext).toBeInstanceOf(Function);
  });
});
