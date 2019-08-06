const assert = require('assert');
const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');

class NewMeasurement extends ProducerFeedItems {

  /**
   * @param {Models.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    assert(
      typeof _.get(measurement, 'getMeasurable') === "function",
      'getMeasurable is required.',
    );
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    await super._preload();
    const measurable = await this.input.getMeasurable();
    this.channelId = _.get(measurable, 'channelId');
    return true;
  }
}

module.exports = {
  NewMeasurement,
};
