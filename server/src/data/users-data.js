const _ = require('lodash');

const { DataBase } = require('./data-base');

const { UserModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class UsersData extends DataBase {

  constructor() {
    super();
    this.UserModel = new UserModel();
  }

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
    return await this.models.User.findOne({
      where: filter
    });
  }

  /**
   * @param {object} data
   * @return {Promise<Models.User>}
   */
  async createOne(data) {
    return await this.models.User.create(data);
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
   * @param id
   * @param data
   * @param _user
   * @return {Promise<Models.User>}
   */
  async updateOne(id, data, _user) {
    let user = await this.models.User.findById(id);
    // @todo: move it into permissions
    if (user && user.auth0Id === _user.auth0Id) {
      user.update(data);
    }
    return user;
  }
}

module.exports = {
  UsersData,
};
