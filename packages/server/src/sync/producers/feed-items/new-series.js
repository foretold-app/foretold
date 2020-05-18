const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewSeries extends ProducerFeedItems {
  /**
   * @param {Defs.Series} series
   */
  constructor(series) {
    super(series);

    this.series = series;
    this.templateName = Producer.TEMPLATE_NAME.NEW_SERIES_FEED_ITEM;
    this.FeedItem = Producer.FeedItemSeries;
  }

  /**
   * @return {Promise.<object>}
   * @protected
   */
  async _getInputs() {
    const seriesId = this.series.id;

    return {
      seriesId,
    };
  }
}

module.exports = {
  NewSeries,
};
