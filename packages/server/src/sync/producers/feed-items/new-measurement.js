const assert = require('assert');
const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

/**
 * @abstract
 */
class NewMeasurement extends ProducerFeedItems {
  /**
   * @param {Defs.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    assert(
      typeof _.get(measurement, 'getMeasurable') === 'function',
      'getMeasurable is required.',
    );
    /** @type {Defs.Measurable} */
    this.measurable = null;
    this.measurement = measurement;
    this.FeedItem = Producer.FeedItemMeasurement;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    await super._preload();
    this.measurable = await this.measurement.getMeasurable();
    this.channelId = _.get(this.measurable, 'channelId', null);
    this.measurementId = _.get(this.measurement, 'id', null);
    return true;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _validateInput() {
    super._validateInput();
    assert(!!this.measurable, 'Measurable is required.');
    assert(!!_.get(this.measurable, 'id'), 'Measurable ID is required.');
    assert(!!this.measurementId, 'Measurement ID is required.');
    return true;
  }

  /**
   * @param {Defs.Agent} agent
   * @return {Promise.<{agent: {
   * name: string},
   * measurable: {id: string, name: string}
   * }>}
   * @protected
   */
  async _getReplacements(agent) {
    const agentName = (await _.get(agent, 'name', null))
      || 'Somebody';
    const measurableName = (await _.get(this.measurable, 'name', null))
      || 'Question';
    const measurableId = _.get(this.measurable, 'id', null);

    return {
      agent: {
        name: agentName,
      },
      measurable: {
        name: measurableName,
        id: measurableId,
      },
      measurement: {
        id: this.measurementId,
      },
    };
  }
}

module.exports = {
  NewMeasurement,
};
