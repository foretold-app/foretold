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
    this.FeedItem = Producer.FeedItemMeasurable;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    await super._preload();
    this.measurable = await this.input.getMeasurable();
    this.channelId = _.get(this.measurable, 'channelId');
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
    return {
      agent: {
        name: (await _.get(agent, 'name')) || 'Somebody',
      },
      measurable: {
        name: (await _.get(this.measurable, 'name')) || 'Question',
        id: _.get(this.measurable, 'id'),
      },
    };
  }
}

module.exports = {
  NewMeasurement,
};
