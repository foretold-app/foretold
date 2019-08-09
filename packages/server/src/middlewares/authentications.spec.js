const index = require('./authentications');

describe('Authentications Middleware', () => {
  it('returns an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
