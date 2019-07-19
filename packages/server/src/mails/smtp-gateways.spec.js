const { SmtpGateways } = require('./smtp-gateways');

describe('Smtp Gateways', () => {
  it('returns an constructor', () => {
    expect(SmtpGateways).toBeInstanceOf(Function);
  });
});
