const _ = require('lodash');
const { Cdf } = require('@foretold/cdf/cdf');
const { CdfCombination } = require('@foretold/cdf/cdf-combination');

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
    const measurementsNotTagged = await this.api.measurementsCompetitive({
      notTaggedByAgent: config.BOT_AGENT_ID,
    });

    if (measurementsNotTagged.length === 0) {
      console.log(`Measurements (not tagged) are empty.`);
      return true;
    }

    console.log(
      `Got "${measurementsNotTagged.length}" not tagged measurements ` +
      `for an aggregation.`
    );

    for (const measurement of measurementsNotTagged) {
      const measurableId = measurement.measurableId;
      const createdAt = measurement.createdAt;

      console.log(
        `Measurable id = "${measurableId}", ` +
        `created at = "${createdAt}".`
      );

      const measurements = await this.api.measurementsCompetitive({
        measurableId,
        findInDateRange: { endDate: createdAt },
      });

      console.log(`Got "${measurements.length}" for aggregation.`);
      if (measurements.length === 0) continue;

      const aggregated = await this.aggregate(measurements);
      if (!aggregated) continue;

      const measurementIds = measurements.map(item => item.id);
      console.log(`Measurement IDs "${measurementIds.join(', ')}".`);

      await this.api.measurementCreateAggregation({ measurableId, ...aggregated });
    }

    return true;
  }

  /**
   * Need to aggregate only "floatCdf".
   * @param {object[]} measurements
   * @return {Promise<{floatCdf: {xs: number[], ys: number[]}} | null>}
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

    const combined = new CdfCombination(cdfs).combine(config.CDF_COMBINE_SIZE);

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
