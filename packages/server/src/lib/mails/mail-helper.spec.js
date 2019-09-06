const { MailHelper } = require('./mail-helper');

describe('Email Config', () => {
  it('returns an constructor', () => {
    expect(MailHelper).toBeInstanceOf(Function);
  });
});
