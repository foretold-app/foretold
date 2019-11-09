const { Samples } = require('./samples');

describe('Samples Class', () => {
  it('toCdf()', () => {
    const samples = new Samples([1,2,3,5]);
    const kde = samples.toCdf({size: 4});
    expect(kde.ys).toStrictEqual([0.25, 0.75, 1.0, 1.0]);
  });
  it('toCdf()', () => {
    const samples = new Samples([1,2,3,5,6,7,8,9,10,12,15,20]);
    const kde = samples.toCdf({size: 4, min: 3, max:8});
    expect(kde.ys).toStrictEqual([
      .25,
      .75,
      1,
      1
    ]);
    expect(kde.xs).toStrictEqual([
      2.3333333333333335,
      4.777777777777778,
      7.222222222222221,
      9.666666666666666,
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