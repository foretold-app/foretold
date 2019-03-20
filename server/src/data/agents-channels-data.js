const _ = require('lodash');

const models = require("../models");

class AgentsChannelsData {

  constructor() {

  }

  /**
   * @param {string} channelId
   * @param {string} agentId
   * @returns {Promise<boolean>}
   */
  async createOne(channelId, agentId) {
    return await models.AgentsChannels.create({
      channelId,
      agentId,
    });
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
