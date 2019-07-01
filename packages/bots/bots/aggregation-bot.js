const _ = require('lodash');

const { API } = require('../api');
const config = require('../config');

const { Aggregation } = require('./aggregation');

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
      const relevantAt = measurement.createdAt;
      const taggedMeasurementId = measurement.id;

      console.log(
        `Measurable id = "${measurableId}", ` +
        `created at = "${createdAt}".`
      );

      const measurements = await this.api.measurementsCompetitive({
        measurableId,
        findInDateRange: { endDate: createdAt },
      });

      const measurementsInOrder = _.orderBy(measurements, ['createdAt'], ['desc']);
      const lastMeasurementOfEachAgent = _.uniqBy(measurementsInOrder, r => r.agentId);

      console.log(`Got "${lastMeasurementOfEachAgent.length}" for aggregation.`);
      if (lastMeasurementOfEachAgent.length === 0) continue;

      const aggregation = new Aggregation(lastMeasurementOfEachAgent);
      const aggregated = await aggregation.main();
      if (!aggregated) {
        console.log(`Nothing to aggregate.`);
        continue;
      }

      const measurementIds = lastMeasurementOfEachAgent.map(item => item.id);
      console.log(`Measurement IDs "${measurementIds.join(', ')}".`);

      await this.api.measurementCreateAggregation({
        measurableId,
        relevantAt,
        taggedMeasurementId,
        ...aggregated
      });
    }

    return true;
  }
}

module.exports = {
  AggregationBot,
};
