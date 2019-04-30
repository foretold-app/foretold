const { interpolate } = require('./functions');

class Cdf {
  /**
   * @param {number[]} xs
   * @param {number[]} ys
   */
  constructor(xs, ys) {
    if (!this.validateSize(xs, ys)) {
      throw new Error('Arrays of "xs" and "ys" have different sizes.');
    }

    const sorted = this.order(xs, ys);
    this.xs = sorted.xs;
    this.ys = sorted.ys;
  }

  /**
   * Order them to make sure that xs are increasing
   * @param {number[]} xs
   * @param {number[]} ys
   * @return {{ys: number[], xs: number[]}}
   */
  order(xs, ys) {
    const xsYs = xs.map((v, i) => ({ ys: ys[i], xs: v }));
    const sorted = xsYs.sort((a, b) => {
      if (a.xs > b.xs) return 1;
      if (a.xs < b.xs) return -1;
      return 0;
    });

    const XS = sorted.map(v => v.xs);
    const YS = sorted.map(v => v.ys);

    return { xs: XS, ys: YS };
  }

  /**
   * @param {number[]} xs
   * @param {number[]} ys
   * @return {boolean}
   */
  validateSize(xs, ys) {
    return xs.length === ys.length;
  }

  /**
   * If xs=[1,2,3], and ys=[5,6,7],
   * then findY(1) = 5, findY(3) = 7, findY(1.5) = 5.5
   * @param x
   * @return {*}
   */
  findY(x) {
    let firstHigherIndex = this.xs.findIndex(X => X >= x);
    let lowerOrEqualXIndex = firstHigherIndex - 1;
    if (lowerOrEqualXIndex < 0) lowerOrEqualXIndex = 0;
    let needsInterpolation = this.xs[lowerOrEqualXIndex] !== x;
    if (needsInterpolation) {
      return interpolate(
        this.xs[lowerOrEqualXIndex],
        this.xs[firstHigherIndex],
        this.ys[lowerOrEqualXIndex],
        this.ys[firstHigherIndex],
        x
      );
    } else {
      return this.ys[lowerOrEqualXIndex];
    }
  }

  /**
   * If xs=[1,2,3], and ys=[5,6,7],
   * then findX(5) = 1, findX(7) = 3, findY(5.5) = 1.5
   * This should do the same thing as `findY`, but for Y.
   * @param y
   * @return {*}
   */
  findX(y) {
    let firstHigherIndex = this.ys.findIndex(Y => Y > y);
    let lowerOrEqualIndex = firstHigherIndex - 1;
    let needsInterpolation = this.ys[lowerOrEqualIndex] !== y;
    if (needsInterpolation) {
      // @todo: should we turn axes?
      return interpolate(
        this.ys[lowerOrEqualIndex],
        this.ys[firstHigherIndex],
        this.xs[lowerOrEqualIndex],
        this.xs[firstHigherIndex],
        y
      );
    } else {
      return this.xs[lowerOrEqualIndex];
    }
  }

  /**
   * @param {number[]} xs
   * @return {Cdf}
   */
  convertWithAlternativeXs(xs) {
    const ys = xs.map(x => this.findY(x));
    return new Cdf(xs, ys);
  }

  /**
   * @param {number} newLength
   * @return {Cdf}
   */
  convertToNewLength(newLength) {
    const _range = range(min(this.xs), max(this.xs), newLength);
    return this.convertWithAlternativeXs(_range);
  }

  /**
   * @return {number}
   */
  sampleSingle() {
    const y = Math.random();
    return this.findX(y);
  }

  /**
   * Poduce n samples, using ``sampleSingle`` for each.
   * @param size
   * @return {number[]}
   */
  sample(size) {
    return Array(size).map(() => this.sampleSingle());
  }

  /**
   * FUTURE
   * TODO: Don't do this one in this story, but it would be nice to do later.
   * @todo: rename to "meanX"
   */
  mean() {

  }
}

module.exports = {
  Cdf,
};
