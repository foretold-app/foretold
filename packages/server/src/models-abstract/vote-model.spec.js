const { VoteModel } = require('./vote-model');

describe('Vote Model', () => {
  it('class should be a constructor', () => {
    expect(VoteModel).toBeInstanceOf(Function);
  });
});
