const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewSeries extends ProducerFeedItems {
  /**
   * @param {Defs.Series} series
   */
  constructor(series) {
    super(series);

    this.templateName = Producer.TEMPLATE_NAME.NEW_SERIES_FEED_ITEM;
    this.FeedItem = Producer.FeedItemSeries;
  }

  /**
   * @param {Defs.Agent} agent
   * @return {Promise.<{
   * agent: {name: string},
   * measurable: {name: string, id: string}
   * }>}
   * @protected
   */
  async _getReplacements(agent) {
    return {
      agent: {
        name: (await _.get(agent, 'name', null)) || 'Somebody',
      },
      series: {
        id: this.input.id,
      },
    };
  }
}

module.exports = {
  NewSeries,
};
