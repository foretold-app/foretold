const _ = require('lodash');

const { DataBase } = require('./data-base');

const { UserModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {UserModel} UserModel
 */
class UsersData extends DataBase {

  constructor() {
    super();
    this.UserModel = new UserModel();
    this.model = this.UserModel;
  }

  /**
   * @param {string} auth0Id
   * @return {Promise<Models.User>}
   */
  async getUserByAuth0Id(auth0Id) {
    return this.upsertOne(
      { auth0Id },
      { auth0Id, name: '' },
    );
  }

  /**
   * @todo: fix interface (params, query, options)
   * @param {object} filter
   * @return {Promise<Models.User>}
   */
  async getOne(filter) {
    return this.models.User.findOne({
      where: filter,
    });
  }

  /**
   * @todo: fix interface
   * @param {object} data
   * @return {Promise<Models.User>}
   */
  async createOne(data) {
    return this.models.User.create(data);
  }

  /**
   * @todo: fix interface
   * @param {object} filter
   * @param {object} data
   * @return {Promise<Models.User>}
   */
  async upsertOne(filter, data) {
    return await this.getOne(filter) || await this.createOne(data);
  }

  /**
   * @todo: fix interface
   * @todo: move to permissions
   * @param id
   * @param data
   * @param _user
   * @return {Promise<Models.User>}
   */
  async updateOne(id, data, _user) {
    let user = await this.models.User.findByPk(id);
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
