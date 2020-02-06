const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../enums');

class FeedItemNotebook extends FeedItemGeneric {
  /**
   * @public
   * @param {object} options
   * @param {string} options.item
   * @param {string} options.description
   */
  constructor(options) {
    super(options);
  }

  /**
   * @public
   * @return {string}
   */
  getName() {
    return FEED_ITEM_BODY.notebook;
  }
}

module.exports = {
  FeedItemNotebook,
};
