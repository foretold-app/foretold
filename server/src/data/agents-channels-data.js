const _ = require('lodash');

const models = require("../models");

class AgentsChannelsData {

  constructor() {

  }

  /**
   * @param {string} channelId
   * @param {string} agentId
   * @returns {Promise<Model>}
   */
  async createOne(channelId, agentId) {
    const agentChannel = await models.AgentsChannels.create({
      channelId,
      agentId,
    });
    return agentChannel;
  }

  /**
   * @param {string} channelId
   * @param {string} agentId
   * @returns {Promise<Model>}
   */
  async deleteOne(channelId, agentId) {
    const agentChannel = await models.AgentsChannels.create({
      channelId,
      agentId,
    });
    if (agentChannel) await agentChannel.destroy();
    return agentChannel;
  }

}

module.exports = {
  AgentsChannelsData,
};
