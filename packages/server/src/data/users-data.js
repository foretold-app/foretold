const assert = require('assert');
const _ = require('lodash');

const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');

const { UserModel } = require('../models-abstract');
const { Params } = require('./classes/params');
const { Query } = require('./classes/query');
const { Data } = require('./classes/data');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {UserModel} UserModel
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
   * @param {string} id
   * @param {Auth0UserInfoResponse} userInfo
   * @return {Promise<Models.User>}
   */
  async updateUserInfoFromAuth0(id, userInfo) {
    const params = new Params({ id });
    const user = await this.getOne(params);

    assert(!!user, 'User is required.');

    const emailIn = _.get(userInfo, 'email');
    const isEmailVerifiedIn = !!_.get(userInfo, 'email_verified');
    const nicknameIn = _.get(userInfo, 'nickname');
    const pictureIn = _.get(userInfo, 'picture');

    const email = _.toString(emailIn).substr(0, 64);
    const nickname = _.toString(nicknameIn).substr(0, 30);
    const picture = _.toString(pictureIn).substr(0, 255);

    const emailValid = email !== '' && isEmailVerifiedIn === true;

    if (user.email === null && emailValid) {
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
   * @param {Models.ObjectID} agentId
   * @return {Promise<number>}
   */
  async getScore(agentId) {
    return this.measurements.getBrierScore(agentId);
  }
}

module.exports = {
  UsersData,
};
