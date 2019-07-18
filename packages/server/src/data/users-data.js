const _ = require('lodash');

const { DataBase } = require('./data-base');
const { MeasurementsData } = require('./measurements-data');

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

    this.measurements = new MeasurementsData();
  }

  /**
   * @param {string} auth0Id
   * @return {Promise<Models.User>}
   */
  async getUserByAuth0Id(auth0Id) {
    const params = { auth0Id };
    const query = {};
    const data = { auth0Id };
    return this.upsertOne(params, query, data);
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
      await user.update(data);
    }
    return user;
  }

  /**
   * @param {string} id
   * @param {Auth0UserInfoResponse} userInfo
   * @return {Promise<Models.User>}
   */
  async updateUserInfoFromAuth0(id, userInfo) {
    const user = await this.models.User.findByPk(id);

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
