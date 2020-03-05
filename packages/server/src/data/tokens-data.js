const crypto = require('crypto');
const moment = require('moment');

const { DataBase } = require('./data-base');

const config = require('../../config/config');
const { TokenModel } = require('./models');
const { TOKEN_TYPE } = require('../enums');

const { Data } = require('./classes');
const { Params } = require('./classes');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {TokenModel} model
 */
class TokensData extends DataBase {
  constructor() {
    super();
    this.model = new TokenModel();
  }

  /**
   * @public
   * @param {string} [token]
   * @return {boolean}
   */
  validate(token = '') {
    const length = token.length === TokensData.MAX_BOT_TOKEN_SIZE_INCOMING;
    const pattern = /^([0-9a-z]+)$/.test(token);
    return length && pattern;
  }

  /**
   * @public
   * @param {string} tokenIn
   * @param {string} [type]
   * @return {Promise<null | string>}
   */
  async getAgentId(tokenIn, type = TOKEN_TYPE.ACCESS_TOKEN) {
    const token = await this._getToken({ type, token: tokenIn });
    if (!token) return null;
    return token.agentId;
  }

  /**
   * @todo: add transaction
   * @public
   * @param {Defs.AgentID} agentId
   * @param {string} [type]
   * @return {Promise<string>}
   */
  async revokeGet(agentId, type = TOKEN_TYPE.ACCESS_TOKEN) {
    await this.model.updateAll(new Params({
      type,
      agentId,
    }), new Data({
      isActive: false,
    }));
    return this.getCreate(agentId, type);
  }

  /**
   * @public
   * @param {string} agentId
   * @param {string} [type]
   * @return {Promise<string>}
   */
  async getCreate(agentId, type = TOKEN_TYPE.ACCESS_TOKEN) {
    let token = await this._getToken({ agentId, type });
    if (!token) token = await this._createToken(agentId, type);
    return token.token;
  }

  /**
   * @public
   * @param {string} [token]
   * @return {Promise<Models.Token>}
   */
  async getAuthToken(token) {
    const type = TOKEN_TYPE.AUTH_TOKEN;
    return this._getToken({ token, type });
  }

  /**
   * @public
   * @param {Defs.AgentID} agentId
   * @return {Promise<Models.Token>}
   */
  async createAuthToken(agentId) {
    const type = TOKEN_TYPE.AUTH_TOKEN;
    const expiresAt = moment.utc().add(3, 'days').toDate();
    const usageCount = 0;
    return this._createToken(agentId, type, expiresAt, usageCount);
  }

  /**
   * @public
   * @param {string} [token]
   * @param {string} [type]
   * @return {Promise<*>}
   */
  async increaseUsageCount(token, type = TOKEN_TYPE.AUTH_TOKEN) {
    return this.model.increaseUsageCount(token, type);
  }

  /**
   * @protected
   * @param {Defs.AgentID} [agentId]
   * @param {string} [token]
   * @param {string} [type]
   * @param {number} [usageCount]
   * @return {Promise<Models.Token>}
   */
  async _getToken({
    agentId, token, type, usageCount,
  }) {
    return this.model.getToken({
      agentId, token, type, usageCount,
    });
  }

  /**
   * @protected
   * @param {Defs.AgentID} agentId
   * @param {string} [type]
   * @param {Date | null} [expiresAt]
   * @param {number | null} [usageCount]
   * @return {Promise<Models.Token>}
   */
  async _createToken(
    agentId,
    type = TOKEN_TYPE.ACCESS_TOKEN,
    expiresAt = null,
    usageCount = null,
  ) {
    return this.createOne(new Data({
      type,
      agentId,
      expiresAt,
      usageCount,
      token: this._generateToken(),
      isActive: true,
    }));
  }

  /**
   * @protected
   * @return {string}
   */
  _generateToken() {
    return crypto.randomBytes(TokensData.MAX_BOT_TOKEN_SIZE).toString('hex');
  }
}

TokensData.MAX_BOT_TOKEN_SIZE = config.MAX_BOT_TOKEN_SIZE;
TokensData.MAX_BOT_TOKEN_SIZE_INCOMING = TokensData.MAX_BOT_TOKEN_SIZE * 2;

module.exports = {
  TokensData,
};
