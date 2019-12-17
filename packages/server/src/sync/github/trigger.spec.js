const { Trigger } = require('./trigger');

describe('GitHub Trigger', () => {
  it('class should be a constructor', () => {
    expect(Trigger).toBeInstanceOf(Object);
  });
});
