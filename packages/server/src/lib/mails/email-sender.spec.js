const { EmailSender } = require('./email-sender');

describe('Email Sender', () => {
  it('returns an constructor', () => {
    expect(EmailSender).toBeInstanceOf(Function);
  });
});
