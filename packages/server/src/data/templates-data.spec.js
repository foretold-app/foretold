const data = require('./templates-data');

describe('Template Data', () => {
  it('class should be a constructor', () => {
    expect(data.TemplatesData).toBeInstanceOf(Function);
  });
});
