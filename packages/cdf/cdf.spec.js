const { Cdf } = require('./cdf');
const { up, down } = require('./functions');

describe('CDF Class', () => {
  it('constructor()', () => {
    const xs = up(1, 9);
    const ys = up(1, 8);
    expect(() => {
      new Cdf(xs, ys);
    }).toThrow(/^Arrays of "xs" and "ys" have different sizes.$/);
  });
  it('order()', () => {
    const xs = down(9, 1);
    const ys = down(9, 1);
    const cdf = new Cdf(xs, ys);
    expect(cdf.xs).toEqual(up(1, 9));
    expect(cdf.ys).toEqual(up(1, 9));
  });
  it('findY()', () => {
    const xs = [1, 2, 3];
    const ys = [5, 6, 7];
    const cdf = new Cdf(xs, ys);
    expect(cdf.findY(1)).toEqual(5);
    expect(cdf.findY(1.5)).toEqual(5.5);
    expect(cdf.findY(3)).toEqual(7);
    expect(cdf.findY(4)).toEqual(7);
    expect(cdf.findY(15)).toEqual(7);
    expect(cdf.findY(-1)).toEqual(5);
  });
  it('findX()', () => {
    const xs = [1, 2, 3];
    const ys = [5, 6, 7];
    const cdf = new Cdf(xs, ys);
    expect(cdf.findX(5)).toEqual(1);
    expect(cdf.findX(7)).toEqual(3);
    expect(cdf.findX(5.5)).toEqual(1.5);
    expect(cdf.findX(8)).toEqual(3);
    expect(cdf.findX(4)).toEqual(1);
  });
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
  it('sample()', () => {
    const xs = up(1, 9);
    const ys = up(70, 78);
    const cdf = new Cdf(xs, ys);
    const XS = cdf.sample(3);
    expect(Number.isInteger(XS[0])).toBe(true);
    expect(Number.isInteger(XS[1])).toBe(true);
    expect(Number.isInteger(XS[2])).toBe(true);
  });
  it('toPdf()', () => {
    const xs = [1,2,5,8];
    const ys = [0.0, 0.4, 0.8, 1.0];
    const cdf = new Cdf(xs, ys);
    const pdf = cdf.toPdf();
    expect(pdf.xs).toStrictEqual(cdf.xs);
    expect(pdf.ys[1]).toBeCloseTo(0.4);
    expect(pdf.ys[2]).toBeCloseTo(0.4);
  });
});
