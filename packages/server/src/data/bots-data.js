const { DataBase } = require('./data-base');

const { BotModel } = require('../models-abstract');
const { TokensData } = require('./tokens-data');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {BotModel} BotModel
 */
class BotsData extends DataBase {

  constructor() {
    super();
    this.BotModel = new BotModel();
    this.model = this.BotModel;
    this.tokens = new TokensData();
  }

  /**
   * @param {object} [data]
   * @return {Promise<*>}
   */
  async createOne(data = {}) {
    return await this.model.createOne(data);
  }

  /**
   * @param {object} [params]
   * @param {object} [data]
   * @return {Promise<*>}
   */
  async updateOne(params = {}, data = {}) {
    return await this.model.updateOne(params, data);
  }

  /**
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Models.ObjectID} filter.userId
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    const restrictions = {};
    return await this.model.getAllWithConnections(filter, pagination, restrictions);
  }

  /**
   * @todo: fix interface (params, query, options*)
   * @param {object} [params]
   * @param {object} [query]
   * @param {object} [restrictions]
   * @return {Promise<*>}
   */
  async getOne(params = {}, query = {}, restrictions = {}) {
    return await this.model.getOne(params, query, restrictions);
  }

  /**
   * @param {object} params
   * @return {Promise<string>}
   */
  async tokenRefresh(params = {}) {
    const bot = await this.BotModel.getOne(params);
    if (!bot) throw new Error('Bot is not found');
    return await this.tokens.revokeTokensAndGetTokenByAgentId(bot.agentId);
  }
}

module.exports = {
  BotsData,
};
