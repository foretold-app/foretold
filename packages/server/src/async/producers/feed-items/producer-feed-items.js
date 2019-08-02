const { Producer } = require('../producer');

class ProducerFeedItems extends Producer {

  constructor(options) {
    super(options);
  }

  /**
   * @param {object} replacements
   * @param {Models.ObjectID} channelId
   * @return {Promise<Models.FeedItem>}
   * @protected
   */
  async _queueFeedItem(replacements, channelId) {
    const template = await this._getTemplate();
    const feedItem = new Producer.FeedItem(template.envelopeTemplate);
    const feedItem$ = feedItem.mutate(replacements);
    return await this._createFeedItem(feedItem$, channelId);
  }

  /**
   * @param {FeedItem} feedItem
   * @param {Models.ObjectID} channelId
   * @return {Promise<*>}
   * @protected
   */
  async _createFeedItem(
    feedItem = new Producer.FeedItem(),
    channelId,
  ) {
    assert(
      feedItem instanceof Producer.FeedItem,
      'feedItem is not FeedItem'
    );
    const data = { body: feedItem, channelId };
    const options = await this._getOptions();
    return Producer.data.feedItems.createOne(data, options);
  }

}

module.exports = {
  ProducerFeedItems,
};
