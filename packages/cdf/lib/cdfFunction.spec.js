const { fnToCdf, integrate } = require('./cdfFunction');
const { Cdf } = require('./cdf');

describe('cdfFunction', () => {
  it('fnToCdf()', () => {
    const cdf1 = new Cdf([0,1,2,3,4,5], [0,1,1,1,2,2]);
    const cdf2 = new Cdf([0,1,2,3,4,5], [0,1,1,1,2,2]);
    let newCdf = fnToCdf([cdf1, cdf2], (a => a[0]*a[1]), 10);
    expect(newCdf.xs.length).toEqual(10);
  })
})