const { TemplateModel } = require('./template-model');

describe('Template Model', () => {
  it('class should be a constructor', () => {
    expect(TemplateModel).toBeInstanceOf(Function);
  });
});
