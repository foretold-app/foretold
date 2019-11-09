const { Cdf } = require('./cdf');
const { up } = require('./functions');

describe('CDF Class', () => {
  it('convertWithAlternativeXs() when "XS" within "xs"', () => {
    const xs = up(1, 9);
    const ys = up(20, 28);
    const cdf = new Cdf(xs, ys);
    const XS = up(3, 7);
    const CDF = cdf.convertWithAlternativeXs(XS);
    expect(CDF.xs).toEqual([3, 4, 5, 6, 7]);
    expect(CDF.ys).toEqual([22, 23, 24, 25, 26]);
  });
  it('convertToNewLength()', () => {
    const xs = up(1, 9);
    const ys = up(50, 58);
    const cdf = new Cdf(xs, ys);
    const CDF = cdf.convertToNewLength(3);
    expect(CDF.xs).toEqual([1, 5, 9]);
    expect(CDF.ys).toEqual([50, 54, 58]);
  });
  it('toPdf()', () => {
    const xs = [1,2,5,8];
    const ys = [0.0, 0.4, 0.8, 1.0];
    const cdf = new Cdf(xs, ys);
    const pdf = cdf.toPdf();
    expect(pdf.xs).toStrictEqual(cdf.xs);
    expect(pdf.ys[1]).toBeCloseTo(0.4);
    expect(pdf.ys[2]).toBeCloseTo(0.13);
  });
  it('combineWithUniformOfCdf()', () => {
    const xs = [1,2,5,8];
    const ys = [0.0, 0.4, 0.8, 1.0];
    const cdf1 = new Cdf([1,2,3,4,5], [0.0, .25, .5, .75, 1.0]);
    const cdf2 = new Cdf([2, 2.1, 2.2, 2.3, 2.4], [0.0, .25, .5, .75, 1.0]);
    const pdf = cdf2.combineWithUniformOfCdf({cdf: cdf1, uniformWeight: .1, sampleCount: 10})
    expect(pdf.ys.length).toBe(10)
    expect(pdf.xs.length).toBe(10)
  });
});
