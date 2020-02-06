const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewNotebook extends ProducerFeedItems {
  /**
   * @param {Models.Notebook} notebook
   */
  constructor(notebook) {
    super(notebook);

    this.templateName = Producer.TEMPLATE_NAME.NEW_NOTEBOOK_FEED_ITEM;
    this.FeedItem = Producer.FeedItemNotebook;
  }
}

module.exports = {
  NewNotebook,
};
