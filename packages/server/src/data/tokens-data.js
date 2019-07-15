const crypto = require('crypto');
const { DataBase } = require('./data-base');

const config = require('../config');

const { TokenModel } = require('../models-abstract');
const { TOKEN_TYPE } = require('../models/enums/token-type');

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
   * @param {string} [token]
   * @return {boolean}
   */
  validate(token = '') {
    const length = token.length === this.MAX_BOT_TOKEN_SIZE_INCOMIN;
    const pattern = /^([0-9a-z]+)$/.test(token);
    return length && pattern;
  }

  /**
   * @param {Models.ObjectID} [agentId]
   * @param {string} [token]
   * @param {string} [type]
   * @return {Promise<Models.Token>}
   */
  async getActiveToken({ agentId, token, type }) {
    type = type || TOKEN_TYPE.ACCESS_TOKEN;
    const cond = { isActive: true, type };
    if (agentId) cond.agentId = agentId;
    if (token) cond.token = token;
    const options = { sort: -1 };
    return this.model.getOne(cond, options);
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {string} [type]
   * @return {Promise<Models.Token>}
   */
  async createActiveToken(agentId, type = TOKEN_TYPE.ACCESS_TOKEN) {
    return this.model.createOne({
      type,
      agentId,
      token: this._getToken(),
      isActive: true,
    });
  }

  /**
   * @param {string} tokenIn
   * @param {string} [type]
   * @return {Promise<null | string>}
   */
  async getAgentIdByToken(tokenIn, type = TOKEN_TYPE.ACCESS_TOKEN) {
    const token = await this.getActiveToken({
      type,
      token: tokenIn,
    });
    if (!token) return null;
    return token.agentId;
  }

  /**
   * @public
   * @param {string} agentId
   * @param {string} [type]
   * @return {Promise<string>}
   */
  async getOrCreateActiveTokenForAgentId(agentId, type = TOKEN_TYPE.ACCESS_TOKEN) {
    let token = await this.getActiveToken({ agentId, type });
    if (!token) token = await this.createActiveToken(agentId, type);
    return token.token;
  }

  /**
   * @todo: add transaction
   * @public
   * @param {string} agentId
   * @param {string} [type]
   * @return {Promise<string>}
   */
  async revokeTokensAndGetTokenByAgentId(agentId, type = TOKEN_TYPE.ACCESS_TOKEN) {
    await this.model.updateAll({
      type,
      agentId,
    }, {
      isActive: false,
    });
    return this.getOrCreateActiveTokenForAgentId(agentId, type);
  }

  /**
   * @protected
   * @return {string}
   */
  _getToken() {
    return crypto.randomBytes(this.MAX_BOT_TOKEN_SIZE).toString('hex');
  }
}

module.exports = {
  TokensData,
};
