const { NewNotebook } = require('./new-notebook');

describe('New Notebook Feed Item Producer', () => {
  it('returns an constructor', () => {
    expect(NewNotebook).toBeInstanceOf(Function);
  });
});
