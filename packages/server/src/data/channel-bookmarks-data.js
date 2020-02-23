const { DataBase } = require('./data-base');
const { ChannelBookmarkModel } = require('../models');

const { Options } = require('../data/classes');
const { Params } = require('../data/classes');
const { Data } = require('../data/classes');
const { Query } = require('../data/classes');

const logger = require('../lib/log');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {VoteModel} model
 */
class ChannelBookmarksData extends DataBase {
  constructor() {
    super();
    this.model = new ChannelBookmarkModel();
    this.log = logger.module('ChannelBookmarksData');
  }

  /**
   * @param {Defs.ChannelID} channelId
   * @param {Defs.AgentID} agentId
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
   * @param {Defs.ChannelID} channelId
   * @param {Defs.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async create(channelId, agentId) {
    const data = new Data({ channelId, agentId });
    const options = new Options({ raw: true });
    return this.createOne(data, options);
  }

  /**
   * @param {Defs.ChannelID} channelId
   * @param {Defs.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async one(channelId, agentId) {
    const params = new Params({ channelId, agentId });
    const query = new Query();
    const options = new Options({ raw: true });
    return this.getOne(params, query, options);
  }

  /**
   * @param {Defs.ChannelID} channelId
   * @param {Defs.AgentID} agentId
   * @returns {Promise<boolean>}
   */
  async delete(channelId, agentId) {
    const params = new Params({ channelId, agentId });
    const query = new Query();
    return this.deleteOne(params, query);
  }
}

module.exports = {
  ChannelBookmarksData,
};
