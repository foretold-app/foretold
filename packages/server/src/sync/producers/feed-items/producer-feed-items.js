const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('../producer');
const logger = require('../../../lib/log');

const { Data } = require('../../../data/classes');
const { Params } = require('../../../data/classes');

const log = logger.module('sync/producers/feed-items');

/**
 * @abstract
 */
class ProducerFeedItems extends Producer {
  /**
   * @param {*} input
   */
  constructor(input) {
    super({});
    assert(_.isObject(input), 'Input should be an object.');

    this.input = input;
    /** @type {Defs.AgentID} */
    this.agentId = null;
    /** @type {Defs.ChannelID} */
    this.channelId = null;

    this.FeedItem = Producer.FeedItemGeneric;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      if (await this._isActual() === false) {
        log.trace(this.constructor.name, 'Hook is not actual');
        return true;
      }
      await this._preload();
      await this._validateInput();
    } catch (e) {
      console.error(this.constructor.name, e.message, e);
      return false;
    }

    try {
      /** @type {Defs.Agent} */
      const agent = await this.agents.getOne(new Params({ id: this.agentId }));
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');

      const replacements = await this._getReplacements(agent);
      await this._queueFeedItem(replacements, this.channelId, this.agentId);

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
    this.agentId = _.get(this.input, 'agentId', null)
      || _.get(this.input, 'creatorId', null)
      || _.get(this.input, 'ownerId', null);
    this.channelId = _.get(this.input, 'channelId', null);
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
   * @param {Defs.Agent} agent
   * @return {Promise.<{agent: {name: string}}>}
   * @protected
   */
  async _getReplacements(agent) {
    return {
      agent: {
        name: (await _.get(agent, 'name', null)) || 'Somebody',
      },
    };
  }

  /**
   * @param {object} replacements
   * @param {Defs.ChannelID} channelId
   * @param {Defs.AgentID | null} agentId
   * @return {Promise<Models.FeedItem>}
   * @protected
   */
  async _queueFeedItem(replacements, channelId, agentId) {
    const template = await this._getTemplate();
    const feedItem = new this.FeedItem(template.envelopeTemplate);
    const feedItem$ = feedItem.instanceFactory(replacements);
    return this._createFeedItem(feedItem$, channelId, agentId);
  }

  /**
   * @param {FeedItem} feedItem
   * @param {Defs.ChannelID} channelId
   * @param {Defs.AgentID | null} agentId
   * @return {Promise<Models.FeedItem>}
   * @protected
   */
  async _createFeedItem(feedItem, channelId, agentId) {
    assert(
      feedItem instanceof Producer.FeedItem,
      'feedItem is not FeedItem.',
    );
    const feedItemBodyName = feedItem.getName();
    const body = { [feedItemBodyName]: feedItem };
    const data = new Data({ body, channelId, agentId });
    const options = await this._getOptions();
    return this.feedItems.createOne(data, options);
  }
}

module.exports = {
  ProducerFeedItems,
};
