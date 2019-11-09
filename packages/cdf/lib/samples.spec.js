const { Samples } = require('./samples');

describe('Samples Class', () => {
  it('toCdf()', () => {
    const samples = new Samples([1,2,3,5]);
    const kde = samples.toCdf({size: 4});
    expect(kde.ys).toStrictEqual([0.25, 0.75, 1.0, 1.0]);
  });
  it('toCdf()', () => {
    const samples = new Samples([1,2,3,5,6,7,8,9,10]);
    const kde = samples.toCdf({size: 4, min: 3, max:8});
    expect(kde.ys).toStrictEqual([
      0.16666666666666666,
      0.5833333333333334,
      0.9166666666666667,
      1
    ]);
    expect(kde.xs).toStrictEqual([
      3,
      4.666666666666667,
      6.333333333333334,
      8,
    ]);
  });
  it('_kde()', () => {
    const samples = new Samples([1,2,3,5]);
    const kde = samples._kde({size: 4});
    let expectedValue = [ { x: -4.333333333333333, y: 0.25 },
      { x: 0.5555555555555554, y: 0.5 },
      { x: 5.444444444444444, y: 0.25 },
      { x: 10.333333333333332, y: 0 } ];
    expect(kde).toStrictEqual(expectedValue);
  });
});