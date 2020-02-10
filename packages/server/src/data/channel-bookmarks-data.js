const { DataBase } = require('./data-base');
const { ChannelBookmarkModel } = require('../models');

const { Options } = require('../data/classes');
const { Params } = require('../data/classes');
const { Data } = require('../data/classes');
const { Query } = require('../data/classes');

const logger = require('../lib/log');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {VoteModel} model
 */
class ChannelBookmarksData extends DataBase {
  constructor() {
    super();
    this.model = new ChannelBookmarkModel();
    this.log = logger.module('ChannelBookmarksData');
  }

  /**
   * @param {Models.ChannelID} channelId
   * @param {Models.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async toggle(channelId, agentId) {
    const channelBookmark = await this.one(channelId, agentId);
    if (!!channelBookmark) {
      await this.delete(channelId, agentId);
      return false;
    }
    await this.create(channelId, agentId);
    return true;
  }

  /**
   * @param {Models.ChannelID} channelId
   * @param {Models.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async create(channelId, agentId) {
    const params = new Params({ channelId, agentId });
    const data = new Data({ channelId, agentId });
    return this.createOne(params, data);
  }

  /**
   * @param {Models.ChannelID} channelId
   * @param {Models.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async one(channelId, agentId) {
    const params = new Params({ channelId, agentId });
    const query = new Query();
    const options = new Options();
    return this.getOne(params, query, options);
  }

  /**
   * @param {Models.ChannelID} channelId
   * @param {Models.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async delete(channelId, agentId) {
    const params = new Params({ channelId, agentId });
    const query = new Query();
    const data = new Data({ channelId, agentId });
    return this.deleteOne(params, query, data);
  }
}

module.exports = {
  ChannelBookmarksData,
};
