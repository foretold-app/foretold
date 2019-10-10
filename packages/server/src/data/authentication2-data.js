const _ = require('lodash');

const { Jwt } = require('../lib/jwt');
const { Auth0 } = require('../lib/auth0');

const { UsersData } = require('./users-data');
const { TokensData } = require('./tokens-data');

const {
  NoUserIdError,
  NotAuthenticatedError,
} = require('./classes/authentication-errors');

/**
 * @todo: Rename it later. Give more meaningfully name.
 */
class Authentication2Data {
  constructor() {
    this.Jwt = new Jwt();
    this.auth0 = new Auth0();

    this.users = new UsersData();
    this.tokens = new TokensData();
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
   * @todo: Remember that all errors in production mode are masked as Internal
   * @todo: errors.
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
  Authentication2Data,
};
