const availability = require('./availability');

describe('Availability Authorizers', () => {
  it('returns availability as an object', () => {
    expect(availability).toBeInstanceOf(Object);
  });
});
