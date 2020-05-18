const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemSeries extends FeedItemGeneric {
  /**
   * @public
   * @param {object} options
   * @param {string} options.item
   * @param {string} options.description
   * @param {Defs.SeriesID} options.seriesId
   */
  constructor(options) {
    super(options);
    assert(_.isString(options.seriesId), 'SeriesId should be a string.');
    this.seriesId = options.seriesId;
  }

  /**
   * @public
   * @return {string}
   */
  getName() {
    return FEED_ITEM_BODY.series;
  }

  /**
   * @public
   * @param {object} replacements
   * @return {FeedItem}
   */
  instanceFactory(replacements) {
    const item = Mustache.render(this.item, replacements);
    const description = Mustache.render(this.description, replacements);
    const seriesId = Mustache.render(this.seriesId, replacements);

    return new FeedItemSeries({
      item,
      description,
      seriesId,
    });
  }
}

module.exports = {
  FeedItemSeries,
};
