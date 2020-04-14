const { context } = require('./context');

describe('Context', () => {
  it('returns a construction', () => {
    expect(context).toBeInstanceOf(Function);
  });
});
