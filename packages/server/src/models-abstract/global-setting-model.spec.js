const { GlobalSettingModel } = require('./global-setting-model');

describe('GlobalSetting Model', () => {
  it('class should be a constructor', () => {
    expect(GlobalSettingModel).toBeInstanceOf(Function);
  });
});
