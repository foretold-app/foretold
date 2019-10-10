const { UsersData } = require('../../data');
const { Auth0 } = require('../../lib/auth0');

class UserUpdater {
  constructor() {
    this.users = new UsersData();
    this.auth0 = new Auth0();
  }

  /**
   * @public
   * @param {Models.User} user
   * @returns {Promise<boolean>}
   */
  async updateUser(user) {
    if (!user.changed('auth0AccessToken')) {
      return false;
    }

    try {
      const userInfo = await this.auth0.getUserInfo(user.auth0AccessToken);
      await this.users.updateUserInfoFromAuth0(user.id, userInfo);
    } catch (e) {
      console.log('Saving user info is failed.', e);
      return false;
    }

    return true;
  }
}

module.exports = {
  UserUpdater,
};
