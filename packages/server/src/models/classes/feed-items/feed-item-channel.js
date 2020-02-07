const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../enums');

class FeedItemChannel extends FeedItemGeneric {
  /**
   * @public
   * @return {string}
   */
  getName() {
    return FEED_ITEM_BODY.channel;
  }
}

module.exports = {
  FeedItemChannel,
};
