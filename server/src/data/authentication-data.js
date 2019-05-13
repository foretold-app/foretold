const jwt = require('jsonwebtoken');

const config = require('../config');

const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');

class AuthenticationData {

  constructor() {
    this.jwt = jwt;

    this.AUTH0_SECRET = config.AUTH0_SECRET;
    this.JWT_SECRET = config.JWT_SECRET;
    this.JWT_ISSUER = config.JWT_ISSUER;
    this.JWT_EXPIN = config.JWT_EXPIN;

    this.users = new UsersData();
    this.agents = new AgentsData();
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
      if (!decoded.sub) throw new Error('No User Id');
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
      if (!agentId) throw new Error('No Agent Id');

      const agent = await this.agents.getOne(agentId);
      if (!agent) throw new Error('Not authenticated');

      const bot = await agent.getBot();
      const user = await agent.getUser();
      const creator = bot || user;

      return { agent, bot, user, creator };
    } catch (err) {
      throw err;
    }
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

}

module.exports = {
  AuthenticationData
};

