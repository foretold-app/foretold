const crypto = require('crypto');
const moment = require('moment');

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
    this.MAX_BOT_TOKEN_SIZE_INCOMING = this.MAX_BOT_TOKEN_SIZE * 2;
  }

  /**
   * @public
   * @param {string} [token]
   * @return {boolean}
   */
  validate(token = '') {
    const length = token.length === this.MAX_BOT_TOKEN_SIZE_INCOMING;
    const pattern = /^([0-9a-z]+)$/.test(token);
    return length && pattern;
  }

  /**
   * @param {string} tokenIn
   * @return {Promise<null | string>}
   */
  async getAgentIdByToken(tokenIn) {
    const token = await this.getToken({ token: tokenIn });
    if (!token) return null;
    return token.agentId;
  }

  /**
   * @todo: add transaction
   * @public
   * @param {string} agentId
   * @param {string} [type]
   * @return {Promise<string>}
   */
  async revokeTokensGetTokenByAgentId(agentId, type = TOKEN_TYPE.ACCESS_TOKEN) {
    await this.model.updateAll({
      type,
      agentId,
    }, {
      isActive: false,
    });
    return this.getCreateTokenByAgentId(agentId, type);
  }

  /**
   * @public
   * @param {string} agentId
   * @param {string} [type]
   * @return {Promise<string>}
   */
  async getCreateTokenByAgentId(agentId, type = TOKEN_TYPE.ACCESS_TOKEN) {
    let token = await this.getToken({ agentId, type });
    if (!token) token = await this.createAccessToken(agentId, type);
    return token.token;
  }

  /**
   * @param {Models.ObjectID} [agentId]
   * @param {string} [token]
   * @param {string} [type]
   * @param {number} [usageCount]
   * @return {Promise<Models.Token>}
   */
  async getToken({ agentId, token, type, usageCount }) {
    return this.model.getToken({ agentId, token, type, usageCount });
  }

  /**
   * @param {Models.ObjectID} agentId
   * @return {Promise<Models.Token>}
   */
  async createAccessToken(agentId) {
    return this.createToken(agentId);
  }

  /**
   * @param {Models.ObjectID} agentId
   * @return {Promise<Models.Token>}
   */
  async createAuthToken(agentId) {
    const type = TOKEN_TYPE.AUTH_TOKEN;
    const expiresAt = moment.utc().add(3, 'days').toDate();
    const usageCount = 0;
    return this.createToken(agentId, type, expiresAt, usageCount);
  }

  /**
   * @param {Models.ObjectID} agentId
   * @param {string} [type]
   * @param {Date | null} [expiresAt]
   * @param {number | null} [usageCount]
   * @return {Promise<Models.Token>}
   */
  async createToken(
    agentId,
    type = TOKEN_TYPE.ACCESS_TOKEN,
    expiresAt = null,
    usageCount = null,
  ) {
    return this.createOne({
      type,
      agentId,
      expiresAt,
      usageCount,
      token: this._getToken(),
      isActive: true,
    });
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
