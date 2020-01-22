const { VotesModel } = require('./votes-model');

describe('Votes Model', () => {
  it('class should be a constructor', () => {
    expect(VotesModel).toBeInstanceOf(Function);
  });
});
