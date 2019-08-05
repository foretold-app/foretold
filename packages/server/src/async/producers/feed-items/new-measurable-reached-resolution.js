const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewMeasurableReachedResolution extends ProducerFeedItems {
  /**
   * @param {Models.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    this.templateName =
      Producer.TEMPLATE_NAME.MEASURABLE_REACHED_RESOLUTION_DATE_FEED_ITEM;
  }

  /**
   * @param {Models.Agent} agent
   * @return {Promise.<{agent: {name: string}}>}
   * @protected
   */
  async _getReplacements(agent) {
    return {
      agent: {
        name: (await _.get(agent, 'name')) || 'Somebody',
      },
      measurable: {
        name: this.input.name || 'Question',
      },
    }
  }
}

module.exports = {
  NewMeasurableReachedResolution,
};
