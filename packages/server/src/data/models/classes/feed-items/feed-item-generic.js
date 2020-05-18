const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItem } = require('./feed-item');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemGeneric extends FeedItem {
  /**
   * @param {object} options
   * @param {string} options.item
   * @param {string} options.description
   */
  constructor(options) {
    super(options);
    assert(!!_.has(options, 'item'), 'Item is required.');
    assert(!!_.has(options, 'description'), 'Description is required.');
    assert(_.isString(options.item), 'Item should be a string.');
    assert(_.isString(options.description), 'Item should be a string.');

    this.item = options.item;
    this.description = options.description;
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

    return new this.constructor({
      item,
      description,
    });
  }
}

module.exports = {
  FeedItemGeneric,
};
