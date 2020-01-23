const assert = require('assert');
const _ = require('lodash');

const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');
const { Params, Query, Data } = require('./classes');

const { UserModel } = require('../models');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {UserModel} model
 */
class UsersData extends DataBase {
  constructor() {
    super();
    this.model = new UserModel();
    this.measurements = new MeasurementsData();
  }

  /**
   * @public
   * @param {string} auth0Id
   * @return {Promise<Models.User>}
   */
  async getUserByAuth0Id(auth0Id) {
    const params = new Params({ auth0Id });
    const query = new Query();
    const data = new Data({ auth0Id });
    return this.upsertOne(params, query, data);
  }

  /**
   * @public
   * @param {Models.UserID} userId
   * @param {Auth0UserInfoResponse} userInfo
   * @return {Promise<Models.User>}
   */
  async updateUserInfoFromAuth0(userId, userInfo) {
    const params = new Params({ id: userId });
    const user = await this.getOne(params);

    assert(!!user, 'User is required.');
    assert(_.isObject(userInfo), 'User Info is required.');

    const emailIn = _.get(userInfo, 'email');
    const isEmailVerifiedIn = !!_.get(userInfo, 'email_verified');
    const nicknameIn = _.get(userInfo, 'nickname');
    const pictureIn = _.get(userInfo, 'picture');

    const email = _.toString(emailIn)
      .substr(0, 64);
    const nickname = _.toString(nicknameIn)
      .substr(0, 30);
    const picture = _.toString(pictureIn)
      .substr(0, 255);

    if (email !== '' && isEmailVerifiedIn === true) {
      user.set('email', email);
      user.set('isEmailVerified', isEmailVerifiedIn);
    }
    if (user.name === '' && nickname !== '') {
      user.set('name', nickname);
    }
    if (user.picture === null && picture !== '') {
      user.set('picture', picture);
    }

    await user.save();
    return user;
  }

  /**
   * @public
   * @param {string} email
   * @return {Promise<Models.User>}
   */
  async getVerified(email) {
    const params = new Params({
      email,
      isEmailVerified: [true],
    });
    return this.getOne(params);
  }

  /**
   * @public
   * @param {Models.UserID} userId
   * @param {string} auth0AccessToken
   * @returns {Promise<*>}
   */
  async saveAccessToken(userId, auth0AccessToken) {
    const params = new Params({ id: userId });
    const data = new Data({ auth0AccessToken });
    return this.updateOne(params, data);
  }
}

module.exports = {
  UsersData,
};
