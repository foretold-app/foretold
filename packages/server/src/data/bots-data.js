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
   * @param {object} data
   * @return {Promise<*>}
   */
  async createOne(data) {
    return await this.BotModel.createOne(data);
  }

  /**
   * @param {object} params
   * @param {object} data
   * @return {Promise<*>}
   */
  async updateOne(params, data) {
    return await this.BotModel.updateOne(params, data);
  }

  /**
   * @todo: fix interface (filter, pagination, options)
   * @param {object} options
   * @return {Promise<*>}
   */
  async getAll(options) {
    return await this.BotModel.getAll(options);
  }

  /**
   * @todo: fix interface (params, query, options*)
   * @param {object} [params]
   * @param {object} [query]
   * @param {object} [restrictions]
   * @return {Promise<*>}
   */
  async getOne(params = {}, query = {}, restrictions = {}) {
    return await this.BotModel.getOne(params, query, restrictions);
  }

  /**
   * @param {object} params
   * @return {Promise<string>}
   */
  async tokenRefresh(params) {
    const bot = await this.BotModel.getOne(params);
    return await this.tokens.revokeTokensAndGetTokenByAgentId(bot.agentId);
  }
}

module.exports = {
  BotsData,
};
