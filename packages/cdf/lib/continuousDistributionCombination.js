const _ = require('lodash');

const { range, min, max, mean } = require('./functions');
const { ContinuousDistribution } = require('./continuousDistribution');

class ContinuousDistributionCombination {
  /**
   * @param {Cdf[]} distributions
   */
  constructor(distributions) {
    this.distributions = distributions;
  }

  /**
   * @return {number}
   */
  minX() {
    return min(this.distributions.map(e => e.minX()))
  }

  /**
   * @return {number}
   */
  maxX() {
    return max(this.distributions.map(e => e.maxX()))
  }

  /**
   * @param {number} xPoint
   * @return {number[]}
   */
  allYsAtXPoint(xPoint) {
    return this.distributions.map(r => r.findY(xPoint));
  }

  /**
   * @param {number} sampleCount
   * @return {number[]}
   */
  equallyDividedXs(sampleCount) {
    return range(this.minX(), this.maxX(), sampleCount);
  }

  /**
   * @param sampleCount
   * @param fn
   * @returns {ContinuousDistribution|*}
   */
  combineYsWithFn(sampleCount, fn) {
    const xs = this.equallyDividedXs(sampleCount);
    const result = xs.map(xPoint => fn(this.allYsAtXPoint(xPoint)));
    let filtered = _.filter(_.zip(xs, result), z => _.isNumber(z[1]));
    let [xss,ys] = _.unzip(filtered);
    return new ContinuousDistribution(xss, ys);
  }

  /**
   * @param {number} sampleCount
   * @return {Cdf}
   */
  combineYsWithMean(sampleCount) {
    return this.combineYsWithFn(sampleCount, mean);
  }
}

module.exports = {
  ContinuousDistributionCombination,
};