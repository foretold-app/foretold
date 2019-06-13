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
    this.MAX_BOT_TOKEN_SIZE = config.MAX_BOT_TOKEN_SIZE;
    this.MAX_BOT_TOKEN_SIZE_INCOMIN = this.MAX_BOT_TOKEN_SIZE * 2;
  }

  /**
   * @public
   * @param {string} token
   * @return {boolean}
   */
  validate(token) {
    const length = token.length === this.MAX_BOT_TOKEN_SIZE_INCOMIN;
    const pattern = /^([0-9a-z]+)$/.test(token);
    return length && pattern;
  }

  /**
   * @param {Models.ObjectID} [agentId]
   * @param {string} [token]
   * @return {Promise<Models.Token>}
   */
  async getActiveToken({ agentId, token }) {
    const cond = { isActive: true };
    if (agentId) cond.agentId = agentId;
    if (token) cond.token = token;
    const options = { sort: -1 };
    return await this.model.getOne(cond, options);
  }

  /**
   * @param {Models.ObjectID} agentId
   * @return {Promise<Models.Token>}
   */
  async createActiveToken(agentId) {
    return await this.model.createOne({
      agentId,
      token: this.getToken(),
      isActive: true,
    });
  }

  /**
   * @param {string} tokenIn
   * @return {Promise<null | string>}
   */
  async getAgentIdByToken(tokenIn) {
    const token = await this.getActiveToken({ token: tokenIn });
    if (!token) return null;
    return token.agentId;
  }

  /**
   * @public
   * @param {string} agentId
   * @return {Promise<string>}
   */
  async getOrCreateActiveTokenForAgentId(agentId) {
    let token = await this.getActiveToken({ agentId });
    if (!token) token = await this.createActiveToken(agentId);
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
    return crypto.randomBytes(this.MAX_BOT_TOKEN_SIZE).toString('hex');
  }
}

module.exports = {
  TokensData,
};
