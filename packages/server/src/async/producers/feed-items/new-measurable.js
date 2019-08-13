const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewMeasurable extends ProducerFeedItems {
  /**
   * @param {Models.Measurable} measurable
   */
  constructor(measurable) {
    super(measurable);
    this.templateName =
      Producer.TEMPLATE_NAME.NEW_MEASURABLE_FEED_ITEM;

    this.FeedItem = Producer.FeedItemMeasurable;
  }

  /**
   * @param {Models.Agent} agent
   * @return {Promise.<{agent: {name: string}, measurable: {name: string, id: string}}>}
   * @protected
   */
  async _getReplacements(agent) {
    return {
      agent: {
        name: (await _.get(agent, 'name')) || 'Somebody',
      },
      measurable: {
        name: this.input.name || 'Question',
        id: this.input.id,
      },
    }
  }
}

module.exports = {
  NewMeasurable,
};
