const _ = require('lodash');
const models = require("../models");

class UsersData {

  /**
   * @param {string} auth0Id
   * @return {Promise<Array<Model>|Model>}
   */
  async auth0User(auth0Id) {
    const users = await models.User.findAll({
      where: {
        auth0Id: auth0Id,
      }
    });
    return users && users[0];
  }

  /**
   * @param options
   * @return {Promise<*>}
   */
  async getAuth0Id(options) {
    const res = await options.user;
    const ok = _.get(res, 'ok');
    const name = _.get(res, 'result.name');
    const sub = _.get(res, 'result.sub');
    if (!ok) throw new Error(name);
    if (!sub) throw new Error("No User Id");
    return sub;
  }

  /**
   * @param root
   * @param values
   * @param options
   * @return {Promise<Model>}
   */
  async editUser(root, values, options) {
    const { id, name } = values;
    let _auth0Id = await this.getAuth0Id(options);
    let user = await models.User.findById(id);
    if (user && user.auth0Id === _auth0Id) {
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
    let _auth0Id = await this.getAuth0Id(options);
    const _auth0User = await this.auth0User(_auth0Id);
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
      user = await models.User.findById(id);
      return user;
    } else if (auth0Id) {
      const user = await this.auth0User(auth0Id);
      return user;
    }
  }
}

module.exports = {
  UsersData,
};
