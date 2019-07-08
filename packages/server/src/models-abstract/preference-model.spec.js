const { PreferencesModel } = require('./preference-model');

describe('Preference Model', () => {
  it('class should be a constructor', () => {
    expect(PreferencesModel).toBeInstanceOf(Function);
  });
});
