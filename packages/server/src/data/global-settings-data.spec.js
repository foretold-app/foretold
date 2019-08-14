const { GlobalSettingsData } = require('./global-settings-data');

describe('Global Setting Data', () => {
  it('class should be a constructor', () => {
    expect(GlobalSettingsData).toBeInstanceOf(Function);
  });
});
