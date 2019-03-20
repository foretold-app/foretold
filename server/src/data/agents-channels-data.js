const _ = require('lodash');

const models = require("../models");

const { UsersData } = require('./users-data');

class AgentsChannelsData {

  constructor() {
    this.usersData = new UsersData();
  }

  /**
   * @param root
   * @param values
   * @param options
   * @returns {Promise<boolean>}
   */
  async createOne(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  /**
   * @param root
   * @param values
   * @param options
   * @returns {Promise<boolean>}
   */
  async deleteOne(root, values, options) {
    console.log(root, values, options);
    return true;
  }

}

module.exports = {
  AgentsChannelsData,
};
