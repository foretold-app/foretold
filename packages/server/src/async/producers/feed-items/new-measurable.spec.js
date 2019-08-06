const { NewMeasurable } = require('./new-measurable');

describe('New Measurable Feed Item Producer', () => {
  it('returns an constructor', () => {
    expect(NewMeasurable).toBeInstanceOf(Function);
  });
});
