const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('../producer');

class ProducerFeedItems extends Producer {

  constructor(input) {
    super({});

    assert(_.isObject(input), 'Input should be an object.');

    this.input = input;
    this.agentId = null;
    this.channelId = null;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      if (await this._isActual() === false) {
        console.log(this.constructor.name, 'Hook is not actual');
        return true;
      }
      await this._preload();
      await this._validateInput();
    } catch (e) {
      console.error(this.constructor.name, e.message, e);
      return false;
    }

    try {
      const agent = await Producer.data.agents.getOne({ id: this.agentId });
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');

      const replacements = await this._getReplacements(agent);
      await this._queueFeedItem(replacements, this.channelId);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      console.error(this.constructor.name, e.message, e);
      return false;
    }
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return true;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _preload() {
    this.agentId = _.get(this.input, 'agentId')
      || _.get(this.input, 'creatorId');
    this.channelId = _.get(this.input, 'channelId');
    return true;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _validateInput() {
    assert(!!this.agentId, 'Agent Id is required.');
    assert(!!this.channelId, 'Channel Id is required.');
    return true;
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
