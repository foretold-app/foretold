const _ = require('lodash');

const { Jwt } = require('../lib/jwt');
const { Auth0 } = require('../lib/auth0');

const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { TokensData } = require('./tokens-data');
const { InvitationsData } = require('./invitations-data');

const {
  NoUserIdError,
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
    this.invitation = new InvitationsData();
  }

  /**
   * @public
   * @param {string} token
   * @return {Promise<{agent: *, creator: *, bot: *, user: *}>}
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
   * @return {Promise<{agent: *, creator: *, bot: *, user: *}>}
   */
  async _byJwt(token) {
    const decoded = this.Jwt.decodeJwtToken(token);
    const agentId = decoded.sub;
    return await this._getContext(agentId);
  }

  /**
   * @protected
   * @param {string} token
   * @return {Promise<{agent: *, creator: *, bot: *, user: *}>}
   */
  async _byToken(token) {
    const agentId = await this.tokens.getAgentId(token);
    return await this._getContext(agentId);
  }

  /**
   * @protected
   * @param {Models.ObjectID} agentId
   * @return {Promise<{agent: *, creator: *, bot: *, user: *}>}
   */
  async _getContext(agentId) {
    if (!agentId) throw new NoAgentIdError();

    const agent = await this.agents.getOne({ id: agentId });
    if (!agent) throw new NotAuthenticatedError();

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
    const decoded = this.Jwt.decodeAuth0Jwt(jwt);
    const auth0Id = _.get(decoded, 'sub');
    if (!auth0Id) {
      throw new NoUserIdError();
    }

    const user = await this.users.getUserByAuth0Id(auth0Id);
    const { agentId } = user;

    // @todo: To move upper?
    try {
      const userInfo = await this.auth0.getUserInfo(accessToken);
      await this.users.updateUserInfoFromAuth0(user.id, userInfo);
    } catch (e) {
      console.log('Saving user info is failed.');
    }

    return this.Jwt.encodeJWT({}, agentId);
  }

  /**
   * @public
   * @todo: To figure out why "NotAuthenticatedError" is not being passed
   * @todo: and is showed as internal error.
   * @param {string} authToken
   * @return {Promise<string>}
   */
  async exchangeAuthToken(authToken) {
    const token = await this.tokens.getAuthToken(authToken);
    if (!token) {
      throw new NotAuthenticatedError();
    }

    await this.tokens.increaseUsageCount(authToken);
    const { agentId } = token;
    return this.Jwt.encodeJWT({}, agentId);
  }
}

module.exports = {
  AuthenticationData,
};
