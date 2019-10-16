const { UsersData } = require('../../data');
const { Auth0 } = require('../../lib/auth0');

const { Filter } = require('../../data/classes');
const logger = require('../../lib/log');

const log = logger.module('sync/actions/user-updater');

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
    if (!user.auth0AccessToken) {
      return false;
    }

    try {
      const userInfo = await this.auth0.getUserInfo(user.auth0AccessToken);
      await this.users.updateUserInfoFromAuth0(user.id, userInfo);
    } catch (e) {
      log.trace('Saving user info is failed.', e);
      return false;
    }

    return true;
  }

  /**
   * @returns {Promise<boolean>}
   */
  async main() {
    const users = await this.getNotVerifiedUsers();

    for (let i = 0, max = users.length; i < max; i++) {
      const user = users[i];
      log.trace(`Update user ID: ${user.id}`);
      await this.updateUser(user);
    }

    return true;
  }

  /**
   * @returns {Promise<Models.User[]>}
   */
  async getNotVerifiedUsers() {
    const filter = new Filter({
      isNotEmailVerified: true,
      notAuth0AccessToken: true,
    });
    return this.users.getAll(filter);
  }
}

module.exports = {
  UserUpdater,
};
