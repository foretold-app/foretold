const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

class FeedItem {
  /**
   * @param {object} options
   * @param {string} options.item
   * @param {string} options.description
   */
  constructor(options = {}) {
    assert(_.isObject(options), 'Options should be an object');
    assert(!!_.get(options, 'item'), 'Item is required');
    assert(!!_.get(options, 'description'), 'Description is required');

    this.item = options.item;
    this.description = options.description;
  }

  /**
   * @param {object} replacements
   * @return {FeedItem}
   */
  mutate(replacements) {
    const item = Mustache.render(this.item, replacements);
    const description = Mustache.render(this.description, replacements);

    return new FeedItem({
      item,
      description,
    });
  }
}

module.exports = {
  FeedItem,
};
