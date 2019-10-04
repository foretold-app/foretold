const _ = require('lodash');

const { Jwt } = require('../lib/jwt');
const { Auth0 } = require('../lib/auth0');

const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { TokensData } = require('./tokens-data');

const {
  NotAuthenticatedError,
  TokenIsInvalidError,
  NoAgentIdError,
} = require('./classes/authentication-errors');

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
   * @return {Promise<{
   *  agent: Models.Agent,
   *  creator: Models.Creator,
   *  bot: Models.Bot,
   *  user: Models.Bot,
   * }>}
   */
  async authenticate(token = '') {
    if (this.tokens.validate(token)) {
      return await this._byToken(token);
    }

    if (this.Jwt.validate(token)) {
      return await this._byJwt(token);
    }

    throw new TokenIsInvalidError();
  }

  /**
   * @protected
   * @param {string} token
   * @return {Promise<{
   *  agent: Models.Agent,
   *  creator: Models.Creator,
   *  bot: Models.Bot,
   *  user: Models.Bot,
   * }>}
   */
  async _byJwt(token) {
    const decoded = this.Jwt.decodeJwtToken(token);
    const agentId = decoded.sub;
    return await this._getContext(agentId);
  }

  /**
   * @protected
   * @param {string} token
   * @return {Promise<{
   *  agent: Models.Agent,
   *  creator: Models.Creator,
   *  bot: Models.Bot,
   *  user: Models.Bot,
   * }>}
   */
  async _byToken(token) {
    const agentId = await this.tokens.getAgentId(token);
    return await this._getContext(agentId);
  }

  /**
   * @protected
   * @param {Models.ObjectID} agentId
   * @return {Promise<{
   *  agent: Models.Agent,
   *  creator: Models.Creator,
   *  bot: Models.Bot,
   *  user: Models.Bot,
   * }>}
   */
  async _getContext(agentId) {
    if (!agentId) {
      throw new NoAgentIdError();
    }

    const agent = await this.agents.getOne({ id: agentId });
    if (!agent) {
      throw new NotAuthenticatedError();
    }

    const bot = await agent.getBot();
    const user = await agent.getUser();
    const creator = bot || user;

    return { agent, bot, user, creator };
  }

}

module.exports = {
  AuthenticationData,
};
