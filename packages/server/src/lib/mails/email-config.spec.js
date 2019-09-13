const { emailConfig } = require('./email-config');

describe('Email Config', () => {
  it('returns an object', () => {
    expect(emailConfig).toBeInstanceOf(Object);
  });
});
