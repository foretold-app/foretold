const R = require('ramda');

const { CDF } = require('./cdf');
const { range, min, max } = require('./functions');

class CdfCombination {
  /**
   * @param {CDF[]} cdfs
   */
  constructor(cdfs) {
    this.cdfs = cdfs;
  }

  /**
   * @return {number}
   */
  minBounds() {
    return min(R.map(e => e.xs[0], this.cdfs))
  }

  /**
   * @return {number}
   */
  maxBounds() {
    return max(R.map(e => e.xs[0], this.cdfs))
  }

  /**
   * @param {number} sampleCount
   * @return {number[]}
   */
  xsToAnalyze(sampleCount) {
    return range(this.minBounds(), this.maxBounds(), sampleCount);
  }

  /**
   * @param {number} xPoint
   * @return {number[]}
   */
  allYsAtXPoint(xPoint) {
    return R.map(r => r.findY(xPoint), this.cdfs);
  }

  /**
   * This assumes equal weighting, which is fine for now
   * @param {number} xPoint
   * @return {number}
   */
  meanOfYsAtXPoint(xPoint) {
    return R.mean(this.allYsAtXPoint(xPoint));
  }

  /**
   * @param {number} sampleCount
   * @return {CDF}
   */
  combine(sampleCount) {
    const xs = this.xsToAnalyze(sampleCount);
    const means = R.map(xPoint => this.meanOfYsAtXPoint(xPoint), xs);
    return new CDF(xs, means);
  }
}

module.exports = {
  CdfCombination,
};