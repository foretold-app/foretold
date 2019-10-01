const { AuthenticationError } = require('apollo-server');

const { Jwt } = require('../lib/jwt');
const { Auth0 } = require('../lib/auth0');

const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { TokensData } = require('./tokens-data');
const { InvitationsData } = require('./invitations-data');

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
    try {
      if (this.tokens.validate(token)) {
        return await this._byToken(token);
      }

      if (this.Jwt.validate(token)) {
        return await this._byJwt(token);
      }

      throw new AuthenticationData.TokenIsInvalidError();
    } catch (err) {
      throw err;
    }
  }

  /**
   * @protected
   * @param {string} token
   * @return {Promise<{agent: *, creator: *, bot: *, user: *}>}
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
   * @return {Promise<{agent: *, creator: *, bot: *, user: *}>}
   */
  async _byToken(token) {
    try {
      const agentId = await this.tokens.getAgentId(token);
      return await this._getContext(agentId);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @protected
   * @param {Models.ObjectID} agentId
   * @return {Promise<{agent: *, creator: *, bot: *, user: *}>}
   */
  async _getContext(agentId) {
    if (!agentId) throw new AuthenticationData.NoAgentIdError();

    const agent = await this.agents.getOne({ id: agentId });
    if (!agent) throw new AuthenticationData.NotAuthenticatedError();

    const bot = await agent.getBot();
    const user = await agent.getUser();
    const creator = bot || user;

    return {
      agent,
      bot,
      user,
      creator,
    };
  }

  /**
   * @todo: To adds "Late User Binding" (with an email).
   * @public
   * @param {string} jwt
   * @param {string} accessToken
   * @return {Promise<string>}
   */
  async exchangeAuthComToken(jwt, accessToken) {
    try {
      const decoded = this.Jwt.decodeAuth0Jwt(jwt);
      if (!decoded.sub) throw new AuthenticationData.NoUserIdError();

      const auth0Id = decoded.sub;
      const user = await this.users.getUserByAuth0Id(auth0Id);
      const { agentId } = user;

      // @todo: To move upper?
      try {
        const userInfo = await this.auth0.getUserInfo(accessToken);
        await this.users.updateUserInfoFromAuth0(user.id, userInfo);
      } catch (e) {
        console.log('Saving user info is failed.');
      }

      // @todo: To move upper?
      try {
        await this.invitation.transition(user);
      } catch (e) {
        console.log('Invitation transition is failed.');
        throw e;
      }

      return this.Jwt.encodeJWT({}, agentId);
    } catch (err) {
      throw err;
    }
  }

  /**
   * @public
   * @todo: To figure out why "NotAuthenticatedError" is not being passed
   * @todo: and is showed as internal error.
   * @param {string} authToken
   * @return {Promise<string>}
   */
  async exchangeAuthToken(authToken) {
    try {
      const token = await this.tokens.getAuthToken(authToken);
      if (!token) throw new AuthenticationData.NotAuthenticatedError();
      await this.tokens.increaseUsageCount(authToken);
      const { agentId } = token;
      return this.Jwt.encodeJWT({}, agentId);
    } catch (err) {
      throw err;
    }
  }
}

AuthenticationData.NoUserIdError
  = class NoUserIdError extends AuthenticationError {
  constructor() {
    super('No User Id');
  }
};

AuthenticationData.NotAuthenticatedError
  = class NotAuthenticatedError extends AuthenticationError {
  constructor() {
    super('Not authenticated');
  }
};

AuthenticationData.TokenIsInvalidError
  = class TokenIsInvalidError extends AuthenticationError {
  constructor() {
    super('Token is invalid');
  }
};

AuthenticationData.NoAgentIdError
  = class NoAgentIdError extends AuthenticationError {
  constructor() {
    super('No Agent Id');
  }
};

module.exports = {
  AuthenticationData,
};
