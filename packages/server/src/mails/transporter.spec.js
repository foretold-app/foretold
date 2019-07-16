const index = require('./transporter');

describe('Transporter', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
