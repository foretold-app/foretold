const { Jwt } = require('../lib/jwt');
const { Auth0 } = require('../lib/auth0');

const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { TokensData } = require('./tokens-data');

class AuthenticationData {

  constructor() {
    this.Jwt = new Jwt();
    this.auth0 = new Auth0();

    this.users = new UsersData();
    this.agents = new AgentsData();
    this.tokens = new TokensData();
  }

  /**
   * @public
   * @param {string} token
   * @return {Promise<*>}
   */
  async authenticate(token = '') {
    try {

      if (this.tokens.validate(token)) {
        return await this._byToken(token);
      }

      if (this.Jwt.validate(token)) {
        return await this._byJwt(token);
      }

      throw new AuthenticationData.TokenIsInvalidError;

    } catch (err) {
      throw err;
    }
  }

  /**
   * @protected
   * @param {string} token
   * @return {Promise<Schema.Context>}
   */
  async _byJwt(token) {
    try {
      const decoded = this.Jwt.decodeJwtToken(token);
      const agentId = decoded.sub;
      return await this._getContext(agentId);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @protected
   * @param {string} token
   * @return {Promise<Schema.Context>}
   */
  async _byToken(token) {
    try {
      const agentId = await this.tokens.getAgentIdByToken(token);
      return await this._getContext(agentId);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @protected
   * @param {Models.ObjectID} agentId
   * @return {Promise<{agent: Models.Agent, creator: *, bot, user}>}
   */
  async _getContext(agentId) {
    if (!agentId) throw new AuthenticationData.NoAgentIdError;

    const agent = await this.agents.getOne(agentId);
    if (!agent) throw new AuthenticationData.NotAuthenticatedError;

    const bot = await agent.getBot();
    const user = await agent.getUser();
    const creator = bot || user;

    return { agent, bot, user, creator };
  }

  /**
   * @public
   * @param {string} jwt
   * @param {string} accessToken
   * @return {Promise<string>}
   */
  async exchangeAuthComToken(jwt, accessToken) {
    try {
      const decoded = this.Jwt.decodeAuth0Jwt(jwt);
      if (!decoded.sub) throw new AuthenticationData.NoUserIdError;

      const user = await this.users.getUserByAuth0Id(decoded.sub);
      const agentId = user.agentId;

      const userInfo = await this.auth0.getUserInfo(accessToken);
      await this.users.updateUserInfo(user.id, userInfo);

      return this.Jwt.encodeJWT({}, agentId);
    } catch (err) {
      throw err;
    }
  }

}

AuthenticationData.NoUserIdError = class NoUserIdError extends Error {
  constructor() {
    super('No User Id');
  }
};

AuthenticationData.NotAuthenticatedError = class NotAuthenticatedError extends Error {
  constructor() {
    super('Not authenticated');
  }
};

AuthenticationData.TokenIsInvalidError = class TokenIsInvalidError extends Error {
  constructor() {
    super('Token is invalid');
  }
};

AuthenticationData.NoAgentIdError = class NoAgentIdError extends Error {
  constructor() {
    super('No Agent Id');
  }
};

module.exports = {
  AuthenticationData,
};
