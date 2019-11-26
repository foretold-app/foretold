const { UsersData } = require('../../data');
const { Auth0 } = require('../../lib/auth0');
const logger = require('../../lib/log');

const { Filter } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');

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

      try {
        const userInfo = await this.auth0.getUserInfo(user.auth0AccessToken);
        await this.users.updateUserInfoFromAuth0(user.id, userInfo);
      } catch (e) {
        log.trace('An access token is expired.', e.message);
        await this.setAuth0AccessTokenAsNull(user.id);
      }

    } catch (e) {
      log.trace('Saving user info is failed.', e.message);
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

  /**
   * @public
   * @returns {Promise<Models.User[]>}
   */
  async setAuth0AccessTokenAsNull(userId) {
    const params = new Params({ id: userId });
    const data = new Data({ auth0AccessToken: null });
    return this.users.updateOne(params, data);
  }
}

module.exports = {
  UserUpdater,
};
