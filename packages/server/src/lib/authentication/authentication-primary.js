const _ = require('lodash');

const { Jwt } = require('../jwt');
const { Auth0 } = require('../auth0');

const { UsersData, TokensData } = require('../../data');

const {
  NoUserIdError,
  NotAuthenticatedError,
} = require('./errors');

class AuthenticationPrimary {
  constructor() {
    this.Jwt = new Jwt();
    this.auth0 = new Auth0();

    this.users = new UsersData();
    this.tokens = new TokensData();
  }

  /**
   * @public
   * @param {string} jwt
   * @param {string} auth0AccessToken
   * @return {Promise<string>}
   */
  async exchangeAuthComToken(jwt, auth0AccessToken) {
    const decoded = this.Jwt.decodeAuth0Jwt(jwt);
    const auth0Id = _.get(decoded, 'sub');
    if (!auth0Id) {
      throw new NoUserIdError();
    }

    const user = await this.users.getUserByAuth0Id(auth0Id);
    await this.users.saveAccessToken(user.id, auth0AccessToken);

    // @todo: To move upper?
    try {
      const userInfo = await this.auth0.getUserInfo(auth0AccessToken);
      await this.users.updateUserInfoFromAuth0(user.id, userInfo);
    } catch (e) {
      console.log('Saving user info is failed.');
    }

    return this.Jwt.encodeJWT({}, user.agentId);
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

    return this.Jwt.encodeJWT({}, token.agentId);
  }
}

module.exports = {
  AuthenticationPrimary,
};
