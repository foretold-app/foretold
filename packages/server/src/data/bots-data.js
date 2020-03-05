const { DataBase } = require('./data-base');
const { TokensData } = require('./tokens-data');

const { BotModel } = require('./models');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {BotModel} model
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
