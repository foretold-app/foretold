const { range } = require('./functions');
const { splitBy } = require('./functions');

describe('Functions', () => {
  it('range()', () => {
    expect(range(1, 5, 3)).toEqual([1, 3, 5]);
    expect(range(1, 5, 5)).toEqual([1, 2, 3, 4, 5]);
    expect(range(-10, 15, 2)).toEqual([-10, 15]);
    expect(range(-10, 15, 3)).toEqual([-10, 2.5, 15]);
    expect(range(-10.3, 17, 3)).toEqual([-10.3, 3.3499999999999996, 17]);
    expect(range(-10.3, 17, 5)).toEqual([-10.3, -3.4750000000000005, 3.3499999999999996, 10.175, 17]);
    expect(range(-10.3, 17.31, 3)).toEqual([-10.3, 3.504999999999999, 17.31]);
    expect(range(1, 1, 3)).toEqual([1, 1, 1]);
  });
  it('splitBy()', () => {
    expect(splitBy(['a', 'b', 'c', 'd', 'e'], 2)).toEqual(['a', 'e']);
    expect(splitBy(['a', 'b', 'c', 'd', 'e'], 3)).toEqual(['a', 'c', 'e']);
  });
});
