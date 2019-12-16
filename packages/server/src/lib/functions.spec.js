const { splitBy } = require('./functions');

describe('Functions', () => {
  it('splitBy()', () => {
    expect(splitBy(['a', 'b', 'c', 'd', 'e'], 2)).toEqual(['a', 'e']);
    expect(splitBy(['a', 'b', 'c', 'd', 'e'], 3)).toEqual(['a', 'c', 'e']);
  });
});
