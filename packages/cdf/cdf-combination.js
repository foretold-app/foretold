const { Cdf } = require('./cdf');
const { range, min, max, mean } = require('./functions');

class CdfCombination {
  /**
   * @param {Cdf[]} cdfs
   */
  constructor(cdfs) {
    this.cdfs = cdfs;
  }

  /**
   * @return {number}
   */
  minBounds() {
    return min(this.cdfs.map(e => e.minX()))
  }

  /**
   * @return {number}
   */
  maxBounds() {
    return max(this.cdfs.map(e => e.maxX()))
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
    return this.cdfs.map(r => r.findY(xPoint));
  }

  /**
   * This assumes equal weighting, which is fine for now
   * @param {number} xPoint
   * @return {number}
   */
  meanOfYsAtXPoint(xPoint) {
    return mean(this.allYsAtXPoint(xPoint));
  }

  /**
   * @param {number} sampleCount
   * @return {Cdf}
   */
  combine(sampleCount) {
    const xs = this.xsToAnalyze(sampleCount);
    const means = xs.map(xPoint => this.meanOfYsAtXPoint(xPoint));
    return new Cdf(xs, means);
  }
}

module.exports = {
  CdfCombination,
};