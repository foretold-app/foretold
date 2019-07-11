const index = require('./mailer');

describe('Mailer', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
