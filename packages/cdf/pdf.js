const { Cdf } = require('./');
const { ContinuousDistribution } = require('./continuousDistribution');

/**
 * @param xs
 * @return {boolean}
 */
function validateHasLength(xs) {
  return xs.length > 0;
}

class Pdf extends ContinuousDistribution{
  /**
   * @param {number[]} xs
   * @param {number[]} ys
   */
  constructor(xs, ys) {
    if (!validateHasLength(xs)) {
      throw new Error('You need at least one element.');
    }
    super(xs, ys);
  }

  /**
   * @return {Cdf}
   */
  toCdf() {
    let newYs = [this.ys[0]];
    for (let i = 1; i < this.ys.length; i++) {
      newYs.push(newYs[i - 1] + this.ys[i])
    }
    return new (require('./cdf').Cdf)(this.xs, newYs);
  }
}

module.exports = {
  Pdf,
};
