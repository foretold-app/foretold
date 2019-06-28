const jwt = require('jsonwebtoken');

const config = require('../config');

const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { TokensData } = require('./tokens-data');

class AuthenticationData {

  constructor() {
    this.jwt = jwt;

    this.AUTH0_SECRET = config.AUTH0_SECRET;
    this.JWT_SECRET = config.JWT_SECRET;
    this.JWT_ISSUER = config.JWT_ISSUER;
    this.JWT_EXPIN = config.JWT_EXPIN;

    this.users = new UsersData();
    this.agents = new AgentsData();
    this.tokens = new TokensData();
  }

  /**
   * @param {string} token
   * @return {boolean}
   */
  validateJwt(token) {
    const pattern = /^[A-Za-z0-9-_=]+\.[A-Za-z0-9-_=]+\.?[A-Za-z0-9-_.+/=]*$/;
    return pattern.test(token);
  }

  /**
   * @param {string} token
   * @return {Promise<boolean>}
   */
  decodeAuth0JwtToken(token) {
    try {
      return this.jwt.verify(token, this.AUTH0_SECRET);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @param {string} token
   * @return {Promise<boolean>}
   */
  decodeJwtToken(token) {
    try {
      return this.jwt.verify(token, this.JWT_SECRET);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @param {object} [payload]
   * @param {string} subject
   * @param {string | null} [expiresIn]
   * @return {string}
   */
  encodeJWT(payload = {}, subject, expiresIn = this.JWT_EXPIN) {
    const options = {
      subject,
      issuer: this.JWT_ISSUER,
    };
    if (expiresIn) options.expiresIn = expiresIn;
    return this.jwt.sign(payload, this.JWT_SECRET, options);
  }

  /**
   * @param {string} token
   * @return {Promise<boolean | Models.User>}
   */
  async authenticationByAuth0JwtToken(token) {
    try {
      const decoded = this.decodeAuth0JwtToken(token);
      if (!decoded.sub) throw new AuthenticationData.NoUserIdError;
      return await this.users.getUserByAuth0Id(decoded.sub);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @param {string} token
   * @return {Promise<Schema.Context>}
   */
  async authenticationByJwtToken(token) {
    try {
      const decoded = this.decodeJwtToken(token);
      const agentId = decoded.sub;
      return await this.getContext(agentId);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @public
   * @param {string} token
   * @return {Promise<Schema.Context>}
   */
  async authenticationByToken(token) {
    try {
      const agentId = await this.tokens.getAgentIdByToken(token);
      return await this.getContext(agentId);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @protected
   * @param {Models.ObjectID} agentId
   * @return {Promise<{agent: Models.Agent, creator: *, bot, user}>}
   */
  async getContext(agentId) {
    if (!agentId) throw new AuthenticationData.NoAgentIdError;

    const agent = await this.agents.getOne(agentId);
    if (!agent) throw new AuthenticationData.NotAuthenticatedError;

    const bot = await agent.getBot();
    const user = await agent.getUser();
    const creator = bot || user;

    return { agent, bot, user, creator };
  }

  /**
   * @param {string} token
   * @return {Promise<string>}
   */
  async getJwtByAuth0Jwt(token) {
    try {
      const user = await this.authenticationByAuth0JwtToken(token);
      const agentId = user.agentId;
      return this.encodeJWT({}, agentId);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @param {string} subject
   * @return {Promise<string>}
   */
  async getJwtForever(subject) {
    try {
      return this.encodeJWT({}, subject, null);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @param {string} token
   * @return {Promise<*>}
   */
  async authenticate(token) {
    try {
      if (this.tokens.validate(token)) {
        return await this.authenticationByToken(token);
      } else if (this.validateJwt(token)) {
        return await this.authenticationByJwtToken(token);
      }
      throw new AuthenticationData.TokenIsInvalidError;
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
