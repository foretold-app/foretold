const crypto = require('crypto');
const { DataBase } = require('./data-base');

const config = require('../config');

const { TokenModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {TokenModel} model
 */
class TokensData extends DataBase {

  constructor() {
    super();
    this.model = new TokenModel();
    this.MAX_TOKEN_SIZE = config.MAX_BOT_TOKEN_SIZE;
  }

  /**
   * @public
   * @param {string} agentId
   * @return {Promise<string>}
   */
  async getOrCreateActiveTokenForAgentId(agentId) {
    let token = await this.model.getOne({
      agentId,
      isActive: true
    }, {
      sort: -1,
    });
    if (!token) token = await this.model.createOne({
      agentId,
      token: this.getToken(),
      isActive: true,
    });
    return token.token;
  }

  /**
   * @todo: add transaction
   * @public
   * @param {string} agentId
   * @return {Promise<string>}
   */
  async revokeTokensAndGetTokenByAgentId(agentId) {
    await this.model.updateAll({
      agentId
    }, {
      isActive: false,
    });
    return await this.getOrCreateActiveTokenForAgentId(agentId);
  }

  /**
   * @protected
   * @return {string}
   */
  getToken() {
    return crypto.randomBytes(this.MAX_TOKEN_SIZE).toString('hex');
  }
}

module.exports = {
  TokensData,
};
