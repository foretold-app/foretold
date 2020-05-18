const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemSeries extends FeedItemGeneric {
  /**
   * @public
   * @param {object} envelopeTemplate
   * @param {string} envelopeTemplate.item
   * @param {string} envelopeTemplate.description
   * @param {object} inputs
   * @param {Defs.SeriesID} inputs.seriesId
   */
  constructor(envelopeTemplate, inputs = {}) {
    super(envelopeTemplate, inputs);

    assert(_.isString(inputs.seriesId),
      'SeriesId should be a string.');

    this.seriesId = inputs.seriesId;
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

    return new FeedItemSeries({
      item,
      description,
    }, {
      seriesId: this.seriesId,
    });
  }
}

module.exports = {
  FeedItemSeries,
};
