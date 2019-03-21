const _ = require('lodash');
const models = require('../models');

class UsersData {

  /**
   * @tested
   * @param {string} auth0Id
   * @return {Promise<Array<Model>|Model>}
   */
  async getUserByAuth0Id(auth0Id) {
    const user = await models.User.findOrCreate({
      where: { auth0Id: auth0Id },
      defaults: { auth0Id: auth0Id, name: '' },
    });
    return user;
  }

  /**
   * @tested
   * @param root
   * @param values
   * @param options
   * @return {Promise<Model>}
   */
  async editUser(root, values, options) {
    const { id, name } = values;
    let user = await models.User.findById(id);
    if (user && user.auth0Id === options.user.auth0Id) {
      user.update({ name });
    }
    return user;
  }

  /**
   * @tested
   * @param ops
   * @param values
   * @param options
   * @return {Promise<*>}
   */
  async getUser(ops, values, options) {
    const { id, auth0Id } = values;
    if (options.user) {
      return options.user;
    } else if (id) {
      return await models.User.findById(id);
    } else if (auth0Id) {
      return await this.getUserByAuth0Id(auth0Id);
    }
  }
}

module.exports = {
  UsersData,
};
