const R = require('ramda');

const { interpolate } = require('./functions');

class Cdf {
  //TODO: This should validate that these are the same length, and should order them to make sure that xs are increasing.
  constructor(xs, ys) {
    this.xs = xs;
    this.ys = ys;
  }


  validate(xs, ys) {
    const sameLength = xs.length === ys.length;
    return sameLength;
  }

  findY(x) {
    let firstHigherIndex = R.findIndex(R.gt(R.__, x))(this.xs);
    let lowerOrEqualXIndex = firstHigherIndex - 1;
    let needsInterpolation = this.xs[lowerOrEqualXIndex] !== x;
    if (needsInterpolation) {
      return interpolate(this.xs[lowerOrEqualXIndex], this.xs[firstHigherIndex], this.ys[lowerOrEqualXIndex], this.ys[firstHigherIndex], x);
    } else {
      return this.ys[lowerOrEqualXIndex];
    }
  }

  //TODO: This should do the same thing as `findY`, but for X.
  findX(y) {

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
