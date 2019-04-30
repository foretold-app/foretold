const _ = require('lodash');
const { Cdf } = require('cdf/cdf');
const { CdfCombination } = require('cdf/cdf-combination');

const { API } = require('../api');

const config = require('../config');

class AggregationBot {
  constructor() {
    this.api = new API(config.BOT_TOKEN);
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    const measurables = await this.api.measurables();
    if (measurables.length === 0) {
      console.log('Measurables list is empty.');
      return true;
    }

    console.log(`Going to process ${measurables.length} measurables.`);

    for (const measurable of measurables) {
      const id = { measurableId: measurable.id };

      const measurements = await this.api.measurementsCompetitive(id);
      console.log(`Going to process ${measurements.length} measurements.`);
      if (measurements.length === 0) continue;

      const aggregated = await this.aggregate(measurements);
      if (!aggregated) continue;

      await this.api.measurementCreateAggregation({ ...id, ...aggregated });
    }

    return true;
  }

  /**
   * @todo: What to do with "floatPoint" spot?
   * @param {object[]} measurements
   * @return {Promise<{floatPoint: number} | {floatCdf: {xs: number[], ys: number[]}} | null>}
   */
  async aggregate(measurements) {
    const cdfs = measurements.filter((measurement) => {
      return !!_.get(measurement, 'value.floatCdf');
    }).map((measurement) => {
      const xs = _.get(measurement, 'value.floatCdf.xs');
      const ys = _.get(measurement, 'value.floatCdf.ys');
      return new Cdf(xs, ys);
    });

    if (cdfs.length === 0) return null;

    // @todo: What's about the limit?
    const combined = new CdfCombination(cdfs).combine(10);

    return {
      floatCdf: {
        xs: combined.xs,
        ys: combined.ys
      },
    };
  }
}

module.exports = {
  AggregationBot,
};
