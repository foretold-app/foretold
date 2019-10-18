const assert = require('assert');
const _ = require('lodash');

const { API } = require('../api');
const config = require('../config');

const { Aggregation } = require('./aggregation');

class Bot {
  constructor() {
    this.api = new API(config.BOT_TOKEN);
    this.mutex = null;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    const agentId = await this._getAgentId();
    assert(!_.isEmpty(agentId), 'Agent ID is required.');

    await this.mutexTake();
    assert(this.mutex !== null, 'Mutex is not free.');

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
      await this.aggregate(measurement);
    }

    await this.mutexFree();
    return true;
  }

  /**
   * @returns {Promise<void>}
   */
  async mutexTake() {
    for (let i = 0, max = 5; i < max; i++) {
      this.mutex = await this.api.mutexTake();
      if (this.mutex) {
        break;
      }
      await this.sleep(3000);
    }
  }

  /**
   * @returns {Promise<void>}
   */
  async mutexFree() {
    await this.api.mutexFree(this.mutex.id);
  }

  /**
   * @param measurement
   * @returns {Promise<void>}
   */
  async aggregate(measurement) {
    console.log(`\x1b[43m --- \x1b[0m`);

    const measurableId = measurement.measurableId;
    const createdAt = measurement.createdAt;
    const relevantAt = measurement.createdAt;
    const taggedMeasurementId = measurement.id;

    console.log(
      `\x1b[43mMeasurable id = "${measurableId}", ` +
      `created at = "${createdAt}".\x1b[0m`
    );

    const lastMeasurementsByAgent
      = await this.getLastMeasurementsByAgent(measurableId, createdAt);

    if (lastMeasurementsByAgent.length === 0) {
      return;
    }

    const aggregated = await new Aggregation(lastMeasurementsByAgent).main();
    if (!aggregated) {
      console.log(`\x1b[43mNothing to aggregate.\x1b[0m`);
      return;
    }

    const measurementIds = lastMeasurementsByAgent.map(item => item.id);

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

  /**
   * @param {string }measurableId
   * @param {string} createdAt
   * @returns {Promise<Array>}
   */
  async getLastMeasurementsByAgent(measurableId, createdAt) {
    const measurements = await this.api.measurementsCompetitive({
      measurableId,
      findInDateRange: { endDate: createdAt },
    });

    const measurementsInOrder = _.orderBy(
      measurements,
      ['createdAt'],
      ['desc'],
    );
    const lastMeasurementsByAgent = _.uniqBy(
      measurementsInOrder,
      r => r.agentId,
    );

    console.log(
      `\x1b[43mGot "${measurementsInOrder.length}" after sorting.\x1b[0m`,
    );
    console.log(
      `\x1b[43mGot "${lastMeasurementsByAgent.length}"`
      + ` for aggregation.\x1b[0m`
    );

    return lastMeasurementsByAgent;
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
   * @param ms
   * @returns {Promise<*>}
   */
  sleep(ms = 3000) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }
}

module.exports = {
  Bot,
};
