const _ = require('lodash');
const models = require('../models');

class UsersData {

  /**
   * @param {string} auth0Id
   * @return {Promise<Array<Model>|Model>}
   */
  async getUserByAuth0Id(auth0Id) {
    const user = await models.User.findOne({
      where: { auth0Id: auth0Id },
    });
    if (!user) throw new Error('User is not found');
    return user;
  }

  /**
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
   * @param ops
   * @param values
   * @param options
   * @return {Promise<*>}
   */
  async getUser(ops, values, options){
    const { id, auth0Id } = values;

    let _auth0Id = options.user.auth0Id;
    const _auth0User = options.user;

    let user;
    if (_auth0Id && !_auth0User) {
      try {
        user = await models.User.create({ auth0Id: _auth0Id, name: "" });
      } catch (e) {
        console.log("E", e);
      }
    }

    if (user) {
      return user;
    } else if (id) {
      return await models.User.findById(id);
    } else if (auth0Id) {
      return await this.auth0User(auth0Id);
    }
  }
}

module.exports = {
  UsersData,
};
