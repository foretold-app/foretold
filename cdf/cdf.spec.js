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
    expect(function () {
      new Cdf(xs, ys);
    }).toThrow(/^Arrays of "xs" and "ys" have different sizes.$/);
  });
  it('constructor()', () => {
    const xs = down(9, 1);
    const ys = up(1, 9);
    const cdf = new Cdf(xs, ys);
    expect(cdf.xs).toEqual(up(1, 9));
    expect(cdf.ys).toEqual(down(9, 1));
  });
});
