const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('../producer');

class ProducerFeedItems extends Producer {

  constructor(input) {
    super({});
    assert(_.isObject(input), 'Input should be an object.');
    assert(!!_.get(input, 'id'), 'Measurable Id is required.');
    assert(!!_.get(input, 'agentId'), 'Agent Id is required.');
    assert(!!_.get(input, 'channelId'), 'Agent Id is required.');
    this.input = input;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      if (await this._isActual() === false) {
        console.log(this.name, 'Hook is not actual');
        return true;
      }
    } catch (e) {
      console.error(this.name, e.message, e);
      return false;
    }

    try {
      const agent = await Producer.data.agents.getOne({
        id: this.input.agentId,
      });
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');

      const replacements = await this._getReplacements(agent);
      const channelId = this.input.channelId;
      await this._queueFeedItem(replacements, channelId);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      console.error(this.name, e.message, e);
      return false;
    }
  }

  /**
   * @param {Models.Agent} agent
   * @return {Promise.<{agent: {name: string}}>}
   * @protected
   */
  async _getReplacements(agent) {
    return {
      agent: {
        name: (await _.get(agent, 'name')) || 'Somebody',
      },
    }
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return false;
  }

  /**
   * @param {object} replacements
   * @param {Models.ObjectID} channelId
   * @return {Promise<Models.FeedItem>}
   * @protected
   */
  async _queueFeedItem(replacements, channelId) {
    const template = await this._getTemplate();
    const feedItem = new Producer.FeedItem(template.envelopeTemplate);
    const feedItem$ = feedItem.mutate(replacements);
    return await this._createFeedItem(feedItem$, channelId);
  }

  /**
   * @param {FeedItem} feedItem
   * @param {Models.ObjectID} channelId
   * @return {Promise<*>}
   * @protected
   */
  async _createFeedItem(feedItem, channelId) {
    assert(
      feedItem instanceof Producer.FeedItem,
      'feedItem is not FeedItem'
    );
    const data = { body: { common: feedItem }, channelId };
    const options = await this._getOptions();
    return Producer.data.feedItems.createOne(data, options);
  }

}

module.exports = {
  ProducerFeedItems,
};
