const R = require('ramda');

const { interpolate } = require('./functions');

class Cdf {
  /**
   * TODO: This should validate that these are the same length, and should order them to make sure that xs are increasing.
   * @param {number[]} xs
   * @param {number[]} ys
   */
  constructor(xs, ys) {
    this.xs = xs;
    this.ys = ys;
  }

  /**
   * @param {number[]} xs
   * @param {number[]} ys
   * @return {boolean}
   */
  validate(xs, ys) {
    const sameLength = xs.length === ys.length;
    return sameLength;
  }

  /**
   * If xs=[1,2,3], and ys=[5,6,7],
   * then findY(1) = 5, findY(3) = 7, findY(1.5) = 5.5
   * @param x
   * @return {*}
   */
  findY(x) {//1
    let firstHigherIndex = R.findIndex(R.gt(R.__, x))(this.xs);
    let lowerOrEqualXIndex = firstHigherIndex - 1;
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
    let firstHigherIndex = R.findIndex(R.gt(R.__, y))(this.ys);
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

  convertWithAlternativeXs(xs) {
    const ys = R.map(this.findY, xs);
    return new Cdf(xs, ys);
  }

  convertToNewLength(newLength) {
    const _range = range(min(this.xs), max(this.xs));
    const ys = this.convertWithAlternativeXs(_range);
  }

  sampleSingle() {
    const yValue = Math.random();
    return this.findX(yValue);
  }

  //TODO: Produce n samples, using ``sampleSingle`` for each.
  sample(sampleSize) {

  }

  //FUTURE TODO: Don't do this one in this story, but it would be nice to do later.
  mean() {

  }
}

module.exports = {
  Cdf,
};
