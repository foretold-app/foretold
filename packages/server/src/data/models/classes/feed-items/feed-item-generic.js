const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItem } = require('./feed-item');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemGeneric extends FeedItem {
  /**
   * @param {object} envelopeTemplate
   * @param {string} envelopeTemplate.item
   * @param {string} envelopeTemplate.description
   * @param {object} inputs
   */
  constructor(envelopeTemplate, inputs = {}) {
    super(envelopeTemplate, inputs);

    assert(!!_.has(envelopeTemplate, 'item'), 'Item is required.');
    assert(!!_.has(envelopeTemplate, 'description'),
      'Description is required.');

    assert(_.isString(envelopeTemplate.item), 'Item should be a string.');
    assert(_.isString(envelopeTemplate.description),
      'Item should be a string.');

    this.item = envelopeTemplate.item;
    this.description = envelopeTemplate.description;
  }

  /**
   * @public
   * @return {string}
   */
  getName() {
    return FEED_ITEM_BODY.generic;
  }

  /**
   * @public
   * @param {object} replacements
   * @return {FeedItem}
   */
  instanceFactory(replacements) {
    const item = Mustache.render(this.item, replacements);
    const description = Mustache.render(this.description, replacements);

    return new FeedItemGeneric({
      item,
      description,
    });
  }
}

module.exports = {
  FeedItemGeneric,
};
