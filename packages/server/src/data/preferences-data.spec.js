const data = require('./preferences-data');

describe('Preferences Base', () => {
  it('class should be a constructor', () => {
    expect(data.PreferencesData).toBeInstanceOf(Function);
  });
});
