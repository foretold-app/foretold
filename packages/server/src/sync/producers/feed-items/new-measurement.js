const assert = require('assert');
const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

/**
 * @abstract
 */
class NewMeasurement extends ProducerFeedItems {
  /**
   * @param {Models.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    assert(
      typeof _.get(measurement, 'getMeasurable') === 'function',
      'getMeasurable is required.',
    );
    /** @type {Models.Measurable} */
    this.measurable = null;
    this.FeedItem = Producer.FeedItemsMeasurement;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    await super._preload();
    this.measurable = await this.input.getMeasurable();
    this.channelId = _.get(this.measurable, 'channelId');
    this.measurementId = _.get(this.input, 'id');
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
   * @param {Models.Agent} agent
   * @return {Promise.<{agent: {
   * name: string},
   * measurable: {id: string, name: string}
   * }>}
   * @protected
   */
  async _getReplacements(agent) {
    const agentName = (await _.get(agent, 'name')) || 'Somebody';
    const measurableName = (await _.get(this.measurable, 'name')) || 'Question';
    const measurableId = _.get(this.measurable, 'id');

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
