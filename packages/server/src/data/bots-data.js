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
   * @public
   * @param {Layers.DataSourceLayer.filter} [filter]
   * @param {Models.ObjectID} filter.userId
   * @param {Layers.DataSourceLayer.pagination} [pagination]
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getAll(filter = {}, pagination = {}, options = {}) {
    const restrictions = {};
    return this.model.getAllWithConnections(filter, pagination, restrictions);
  }

  /**
   * @param {object} params
   * @return {Promise<string>}
   */
  async tokenRefresh(params = {}) {
    const bot = await this.BotModel.getOne(params);
    if (!bot) throw new Error('Bot is not found');
    return this.tokens.revokeGet(bot.agentId);
  }
}

module.exports = {
  BotsData,
};
