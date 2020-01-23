const { Model } = require('./model');

describe('Model', () => {
  it('class should be a constructor', () => {
    expect(Model).toBeInstanceOf(Function);
  });
});
