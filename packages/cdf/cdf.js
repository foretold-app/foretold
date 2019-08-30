const { interpolate, range, min, max } = require('./functions');
const { ContinuousDistribution } = require('./continuousDistribution');

class Cdf extends ContinuousDistribution{
  /**
   *
   * @return {Pdf}
   */
  toPdf() {
    let newYs = [this.ys[0]];
    for (let i = 1; i < this.ys.length; i++) {
      newYs.push(this.ys[i] - this.ys[i - 1])
    }
    return new (require('./pdf').Pdf)(this.xs, newYs);
  }
}

module.exports = {
  Cdf,
};
