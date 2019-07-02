const _ = require('lodash');

const { Cdf } = require('@foretold/cdf/cdf');
const { CdfCombination } = require('@foretold/cdf/cdf-combination');

const config = require('../config');

class Aggregation {
  constructor(measurements) {
    this.measurements = measurements;
  }

  /**
   * @return {Promise<{floatCdf: {xs: number[], ys: number[]}}|{percentage: number}>}
   */
  async main() {
    return await this.aggregateFloatCdf()
      || await this.aggregatePercentage();
  }

  /**
   * @return {Promise<{floatCdf: {xs: number[], ys: number[]}} | null>}
   */
  async aggregateFloatCdf() {
    return this._aggregateFloatCdf(this.measurements);
  }

  /**
   * Need to aggregate only "floatCdf".
   * @param {object[]} measurements
   * @return {Promise<{floatCdf: {xs: number[], ys: number[]}} | null>}
   */
  async _aggregateFloatCdf(measurements) {
    const values = measurements.filter((measurement) => {
      return !!_.get(measurement, 'value.floatCdf');
    }).map((measurement) => {
      const xs = _.get(measurement, 'value.floatCdf.xs');
      const ys = _.get(measurement, 'value.floatCdf.ys');
      return new Cdf(xs, ys);
    });

    if (values.length === 0) return null;

    const combined = new CdfCombination(values)
      .combine(config.CDF_COMBINE_SIZE);

    return {
      floatCdf: {
        xs: combined.xs,
        ys: combined.ys,
      },
    };
  }

  /**
   * @return {Promise<{percentage: number} | null>}
   */
  async aggregatePercentage() {
    return this._aggregatePercentage(this.measurements);
  }

  /**
   * Need to aggregate only "percentages".
   * @param {object[]} measurements
   * @return {Promise<{percentage: number} | null>}
   */
  async _aggregatePercentage(measurements) {
    const values = measurements.map((measurement) => {
      return _.get(measurement, 'value.percentage');
    }).filter((value) => {
      return value !== null && value !== undefined;
    });

    if (values.length === 0) return null;

    const mean = _.mean(values);
    return { percentage: mean };
  }
}

module.exports = {
  Aggregation,
};
