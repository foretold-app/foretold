const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewChannel extends ProducerFeedItems {
  /**
   * @param {Definitions.Channel} channel
   */
  constructor(channel) {
    super(channel);

    this.templateName = Producer.TEMPLATE_NAME.NEW_CHANNEL_FEED_ITEM;
    this.FeedItem = Producer.FeedItemChannel;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    this.agentId = _.get(this.input, 'creatorId', null);
    this.channelId = _.get(this.input, 'id', null);
    return true;
  }
}

module.exports = {
  NewChannel,
};
