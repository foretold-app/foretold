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
    let { ok, result } = await options.user;
    if (!ok) throw new Error(result.name);
    let { sub } = result;
    if (!sub) throw new Error("No User Id");
    return sub;
  }
}

module.exports = {
  UsersData,
};
