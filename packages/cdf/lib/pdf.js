const { Cdf } = require('./cdf');
const { ContinuousDistribution } = require('./continuousDistribution');
const _ = require('lodash');

/**
 * @param xs
 * @return {boolean}
 */
function validateHasLength(xs) {
  return xs.length > 0;
}

class Pdf extends ContinuousDistribution{
  /**
   * @return {Cdf}
   */
  toCdf() {
    let newYs = [_.isFinite(this.ys[0]) ? this.ys[0] : 0];
    for (let i = 1; i < this.ys.length; i++) {
      let newValue = _.isFinite(this.ys[i]) ? newYs[i - 1] + this.ys[i] : newYs[i - 1];
      newYs.push(newValue);
    }
    return new Cdf(this.xs, newYs);
  }
}

module.exports = {
  Pdf,
};
