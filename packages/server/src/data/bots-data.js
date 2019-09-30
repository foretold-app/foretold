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
    this.model = new BotModel();
    this.tokens = new TokensData();
  }

  /**
   * @public
   * @param {object} params
   * @return {Promise<string>}
   */
  async tokenRefresh(params = {}) {
    const bot = await this.getOne(params);
    if (!bot) throw new Error('Bot is not found');
    return this.tokens.revokeGet(bot.agentId);
  }
}

module.exports = {
  BotsData,
};
