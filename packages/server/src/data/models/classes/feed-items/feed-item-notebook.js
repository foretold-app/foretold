const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemNotebook extends FeedItemGeneric {
  /**
   * @public
   * @param {object} options
   * @param {string} options.item
   * @param {string} options.description
   * @param {Defs.NotebookID} options.notebookId
   */
  constructor(options) {
    super(options);
    assert(_.isString(options.notebookId), 'NotebookId should be a string.');
    this.notebookId = options.notebookId;
  }

  /**
   * @public
   * @return {string}
   */
  getName() {
    return FEED_ITEM_BODY.notebook;
  }

  /**
   * @public
   * @param {object} replacements
   * @return {FeedItem}
   */
  instanceFactory(replacements) {
    const item = Mustache.render(this.item, replacements);
    const description = Mustache.render(this.description, replacements);
    const notebookId = Mustache.render(this.notebookId, replacements);

    return new this.constructor({
      item,
      description,
      notebookId,
    });
  }
}

module.exports = {
  FeedItemNotebook,
};
