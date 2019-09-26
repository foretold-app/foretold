const { Emails } = require('./emails');

describe('Emails', () => {
  it('returns an constructor', () => {
    expect(Emails).toBeInstanceOf(Function);
  });
});
