const { Cdf } = require('./cdf');

function up(from, to) {
  const arr = [];
  for (let i = from; i <= to; i++) arr.push(i);
  return arr;
}

function down(from, to) {
  const arr = [];
  for (let i = from; i >= to; i--) arr.push(i);
  return arr;
}

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
    const ys = up(1, 9);
    const cdf = new Cdf(xs, ys);
    expect(cdf.xs).toEqual(up(1, 9));
    expect(cdf.ys).toEqual(down(9, 1));
  });
  it('findY()', () => {
    const xs = [1, 2, 3];
    const ys = [5, 6, 7];
    const cdf = new Cdf(xs, ys);
    expect(cdf.findY(1)).toEqual(5);
    expect(cdf.findY(3)).toEqual(7);
    expect(cdf.findY(1.5)).toEqual(5.5);
  });
  it('findX()', () => {
    const xs = [1, 2, 3];
    const ys = [5, 6, 7];
    const cdf = new Cdf(xs, ys);
    expect(cdf.findX(5)).toEqual(1);
    expect(cdf.findX(7)).toEqual(3);
    expect(cdf.findX(5.5)).toEqual(1.5);
  });
});
