const { NotebookBookmarkModel } = require('./notebook-bookmark-model');

describe('Notebook Bookmark Model', () => {
  it('class should be a constructor', () => {
    expect(NotebookBookmarkModel).toBeInstanceOf(Function);
  });
});
