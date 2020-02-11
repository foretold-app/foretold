const index = require('./middlewares');

describe('Middlewares Module', () => {
  it('index is an object', () => {
    expect(index).toBeInstanceOf(Object);
  });
});
