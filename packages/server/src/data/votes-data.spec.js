const data = require('./votes-data');

describe('Votes Data', () => {
  it('class should be a constructor', () => {
    expect(data.VotesData).toBeInstanceOf(Function);
  });
});
