const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemGeneric } = require('./feed-item-generic');
const { FEED_ITEM_BODY } = require('../../../../enums');

class FeedItemNotebook extends FeedItemGeneric {
  /**
   * @public
   * @param {object} envelopeTemplate
   * @param {string} envelopeTemplate.item
   * @param {string} envelopeTemplate.description
   * @param {inputs} inputs
   * @param {Defs.NotebookID} inputs.notebookId
   */
  constructor(envelopeTemplate, inputs) {
    super(envelopeTemplate, inputs);

    assert(_.isString(inputs.notebookId),
      'NotebookId should be a string.');

    this.notebookId = inputs.notebookId;
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

    return new FeedItemNotebook({
      item,
      description,
    }, {
      notebookId: this.notebookId,
    });
  }
}

module.exports = {
  FeedItemNotebook,
};
