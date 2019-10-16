const index = require('./index');

describe('index', () => {
  it('should return valid object', () => {
    expect(!!index)
      .toBe(true);
  });
});
