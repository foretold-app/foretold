const data = require('./notebooks-data');

describe('Notebooks Data', () => {
  it('class should be a constructor', () => {
    expect(data.NotebooksData).toBeInstanceOf(Function);
  });
});
