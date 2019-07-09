const { PreferenceModel } = require('./preference-model');

describe('Preference Model', () => {
  it('class should be a constructor', () => {
    expect(PreferenceModel).toBeInstanceOf(Function);
  });
});
