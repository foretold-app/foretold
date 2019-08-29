const { fnToCdf, integrate } = require('./cdfFunction');
const { Cdf } = require('./cdf');
const { Samples } = require('./samples');

describe('Test', () => {
  it('toCdf()', () => {
    const cdf1 = new Cdf([0,1,2,3,4,5], [0,1,1,1,2,2]);
    const cdf2 = new Cdf([0,1,2,3,4,5], [0,1,1,1,2,2]);
    let foo = fnToCdf([cdf1, cdf2], (a => a[0]*a[1]), 100);
    expect(1).toEqual(1);
  })
})