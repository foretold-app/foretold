const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewChannel extends ProducerFeedItems {
  /**
   * @param {Defs.Channel} channel
   */
  constructor(channel) {
    super(channel);

    this.channel = channel;
    this.templateName = Producer.TEMPLATE_NAME.NEW_CHANNEL_FEED_ITEM;
    this.FeedItem = Producer.FeedItemChannel;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    this.agentId = _.get(this.channel, 'creatorId', null);
    this.channelId = _.get(this.channel, 'id', null);
    return true;
  }
}

module.exports = {
  NewChannel,
};
