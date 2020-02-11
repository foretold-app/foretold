const assert = require('assert');
const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');

class NewNotebook extends ProducerFeedItems {
  /**
   * @param {Models.Notebook} notebook
   */
  constructor(notebook) {
    super(notebook);

    this.templateName = Producer.TEMPLATE_NAME.NEW_NOTEBOOK_FEED_ITEM;
    this.FeedItem = Producer.FeedItemNotebook;

    this.notebookId = null;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    await super._preload();
    this.notebookId = _.get(this.input, 'id', null);
    return true;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _validateInput() {
    super._validateInput();
    assert(!!this.notebookId, 'Notebook ID is required.');
    return true;
  }

  /**
   * @param {Models.Agent} agent
   * @return {Promise.<{
   * agent: { name: string },
   * notebook: { id: string }
   * }>}
   * @protected
   */
  async _getReplacements(agent) {
    const agentName = (await _.get(agent, 'name', null)) || 'Somebody';

    return {
      agent: {
        name: agentName,
      },
      notebook: {
        id: this.notebookId,
      },
    };
  }
}

module.exports = {
  NewNotebook,
};
