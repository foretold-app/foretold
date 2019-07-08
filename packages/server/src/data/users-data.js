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
    return await this.getOne(filter)
      || await this.createOne(data);
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
    const user = await this.models.User.findByPk(id);
    if (user && user.auth0Id === _user.auth0Id) {
      user.update(data);
    }
    return user;
  }

  /**
   * @param {string} id
   * @param {Auth0UserInfoResponse} userInfo
   * @return {Promise<Models.User>}
   */
  async updateUserInfo(id, userInfo) {
    const user = await this.models.User.findByPk(id);

    const emailIn = _.get(userInfo, 'email');
    const emailVerifiedIn = _.get(userInfo, 'email_verified') || false;
    const nicknameIn = _.get(userInfo, 'nickname');
    const pictureIn = _.get(userInfo, 'picture');

    const email = _.toString(emailIn).substr(0, 64);
    const nickname = _.toString(nicknameIn).substr(0, 32);
    const picture = _.toString(pictureIn).substr(0, 128);

    if (email !== '' && emailVerifiedIn === true) user.set('email', email);
    if (nickname !== '') user.set('nickname', nickname);
    if (picture !== '') user.set('picture', picture);

    await user.save();

    return user;
  }
}

module.exports = {
  UsersData,
};
