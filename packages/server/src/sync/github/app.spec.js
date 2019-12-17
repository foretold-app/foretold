const { app } = require('./app');

describe('GitHub App', () => {
  it('class should be a object', () => {
    expect(app).toBeInstanceOf(Object);
  });
});
