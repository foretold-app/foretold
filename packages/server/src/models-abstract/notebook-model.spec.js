const { NotebookModel } = require('./notebook-model');

describe('Notebook Model', () => {
  it('class should be a constructor', () => {
    expect(NotebookModel).toBeInstanceOf(Function);
  });
});
