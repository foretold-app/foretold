const auth0 = require('auth0');
const config = require('../config');

class Auth0 {
  constructor() {
    this.apiClient = new auth0.AuthenticationClient({
      domain: config.AUTH0_DOMAIN,
      clientId: config.AUTH0_CLIENT_ID,
    });
  }

  /**
   * @public
   * @return {Promise.<Auth0UserInfoResponse | *>}
   */
  getUserInfo(accessToken) {
    return this.apiClient.getProfile(accessToken)
      .catch((err) => {
        console.log('getUserInfo', err);
        return {};
      });
  }
}

module.exports = {
  Auth0,
};
