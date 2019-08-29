const { range, min, max, mean } = require('./functions');
const { ContinuousDistribution } = require('./continuousDistribution');

class CdfCombination {
  /**
   * @param {Cdf[]} distributions
   */
  constructor(distributions) {
    this.distributions = distributions;
  }

  /**
   * @return {number}
   */
  minBounds() {
    return min(this.distributions.map(e => {return e.minX()}))
  }

  /**
   * @return {number}
   */
  maxBounds() {
    return max(this.distributions.map(e => e.maxX()))
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
    return this.distributions.map(r => r.findY(xPoint));
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
    return new ContinuousDistribution(xs, means);
  }
}

module.exports = {
  CdfCombination,
};