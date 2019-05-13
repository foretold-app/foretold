const crypto = require('crypto');
const { DataBase } = require('./data-base');

const { TokenModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {TokenModel} model
 */
class TokensData extends DataBase {

  constructor() {
    super();
    this.model = new TokenModel();
  }

  /**
   * @param {string} agentId
   * @return {Promise<Models.Token>}
   */
  async getOrCreateActiveTokenForAgentId(agentId) {
    const token = await this.model.getOne({
      agentId,
      isActive: true
    });
    if (token) return token;
    return await this.model.createOne({
      token: this.getToken(),
      isActive: true,
    });
  }

  /**
   * @todo: add transaction
   * @param {string} agentId
   * @return {Promise<Models.Token>}
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
   * @return {string}
   */
  getToken() {
    return crypto.randomBytes(64).toString('hex');
  }
}

module.exports = {
  TokensData,
};
