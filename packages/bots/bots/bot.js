const assert = require('assert');
const _ = require('lodash');

const { API } = require('../api');
const config = require('../config');

const { Aggregation } = require('./aggregation');

class Bot {
  constructor() {
    this.api = new API(config.BOT_TOKEN);
  }

  /**
   * @protected
   * @return {Promise<string>}
   */
  async _getAgentId() {
    const authenticated = await this.api.queryAuthenticated();
    return _.get(authenticated, 'agent.id');
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    const agentId = await this._getAgentId();
    assert(!_.isEmpty(agentId), 'Agent ID is required.');

    const measurementsNotTagged = await this.api.measurementsCompetitive({
      notTaggedByAgent: agentId,
    });

    if (measurementsNotTagged.length === 0) {
      console.log(`\x1b[43mMeasurements (not tagged) are empty.\x1b[0m`);
      return true;
    }

    console.log(`\x1b[43m --------------------------------- \x1b[0m`);
    console.log(
      `\x1b[43mGot "${measurementsNotTagged.length}" not tagged measurements ` +
      `for an aggregation.\x1b[0m`
    );

    for (const measurement of measurementsNotTagged) {

      console.log(`\x1b[43m --- \x1b[0m`);

      const measurableId = measurement.measurableId;
      const createdAt = measurement.createdAt;
      const relevantAt = measurement.createdAt;
      const taggedMeasurementId = measurement.id;

      console.log(
        `\x1b[43mMeasurable id = "${measurableId}", ` +
        `created at = "${createdAt}".\x1b[0m`
      );

      const measurements = await this.api.measurementsCompetitive({
        measurableId,
        findInDateRange: { endDate: createdAt },
      });

      const measurementsInOrder = _.orderBy(
        measurements,
        ['createdAt'],
        ['desc'],
      );
      const lastMeasurementsOfEachAgent = _.uniqBy(
        measurementsInOrder,
        r => r.agentId,
      );

      console.log(
        `\x1b[43mGot "${measurementsInOrder.length}" after sorting.\x1b[0m`,
      );
      console.log(
        `\x1b[43mGot "${lastMeasurementsOfEachAgent.length}"`
        + ` for aggregation.\x1b[0m`
      );

      if (lastMeasurementsOfEachAgent.length === 0) {
        continue;
      }

      const aggregation = new Aggregation(lastMeasurementsOfEachAgent);
      const aggregated = await aggregation.main();
      if (!aggregated) {
        console.log(`\x1b[43mNothing to aggregate.\x1b[0m`);
        continue;
      }

      const measurementIds = lastMeasurementsOfEachAgent.map(item => item.id);

      console.log(
        `\x1b[43mMeasurement IDs "${measurementIds.join(', ')}".\x1b[0m`,
      );

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
  Bot,
};
