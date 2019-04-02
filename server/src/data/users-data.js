const _ = require('lodash');
const models = require('../models');

const { DataBase } = require('./data-base');

class UsersData extends DataBase {

  /**
   * @param {string} auth0Id
   * @return {Promise<Array<Model>|Model>}
   * @todo change to more generic getOne(filter)
   */
  async getUserByAuth0Id(auth0Id) {
    const user = await models.User.findOne({
      where: { auth0Id: auth0Id }
    }) || await models.User.create({
      auth0Id: auth0Id, name: '',
    });
    return user;
  }

  /**
   * @return {Promise<{name: string}>}
   */
  async getGuestUserAsLiteral() {
    return { name: 'Guest' };
  }

  /**
   * @param root
   * @param values
   * @param options
   * @return {Promise<Model>}
   */
  async updateOne(root, values, options) {
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
