const _ = require('lodash');
const models = require('../models');

const { DataBase } = require('./data-base');

class UsersData extends DataBase {

  /**
   * @param {string} auth0Id
   * @return {Promise<Models.User>}
   */
  async getUserByAuth0Id(auth0Id) {
    return await this.upsertOne(
      { auth0Id },
      { auth0Id, name: '' },
    );
  }

  /**
   * @param {object} filter
   * @return {Promise<Models.User>}
   */
  async getOne(filter) {
    return await models.User.findOne({
      where: filter
    });
  }

  /**
   * @param {object} data
   * @return {Promise<Models.User>}
   */
  async createOne(data) {
    return await models.User.create(data);
  }

  /**
   * @param {object} filter
   * @param {object} data
   * @return {Promise<Models.User>}
   */
  async upsertOne(filter, data) {
    return await this.getOne(filter) || await this.createOne(data);
  }

  /**
   * @return {Promise<{name: string}>}
   */
  async getGuestUserAsLiteral() {
    return { name: 'Guest' };
  }

  /**
   * @param id
   * @param data
   * @param _user
   * @return {Promise<Models.User>}
   */
  async updateOne(id, data, _user) {
    let user = await models.User.findById(id);
    if (user && user.auth0Id === _user.auth0Id) {
      user.update(data);
    }
    return user;
  }
}

module.exports = {
  UsersData,
};
