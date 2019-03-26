const _ = require('lodash');

const models = require("../models");

class AgentsChannelsData {

  /**
   * @tested
   * @param {string} channelId
   * @param {string} agentId
   * @returns {Promise<Model>}
   */
  async createOne(channelId, agentId) {
    const input = { channelId, agentId };
    await this.validate(input);
    const agentChannel =
      await models.AgentsChannels.findOne({ where: input }) ||
      await models.AgentsChannels.create(input);
    return agentChannel;
  }

  /**
   * @tested
   * @param {string} channelId
   * @param {string} agentId
   * @returns {Promise<Model>}
   */
  async deleteOne(channelId, agentId) {
    const input = { channelId, agentId };
    await this.validate(input);
    const agentChannel = await models.AgentsChannels.findOne({ where: input });
    if (agentChannel) {
      await models.AgentsChannels.destroy({ where: input });
    }
    return agentChannel;
  }

  /**
   * @tested
   * @param {string} channelId
   * @param {string} agentId
   * @return {Promise<*>}
   */
  async validate({ channelId, agentId }) {
    if (!await models.Channel.findById(channelId)) {
      return Promise.reject(new Error(`Channel "${ channelId }" is not found.`));
    }
    if (!await models.Agent.findById(agentId)) {
      return Promise.reject(new Error(`Agent "${ agentId }" is not found.`));
    }
    return true;
  }

  /**
   * @tested
   * @param {object} options
   * @param {string} options.agentId?
   * @param {string} options.channelId?
   * @returns {Promise<Model>}
   */
  async getOne(options) {
    return await models.AgentsChannels.findOne({ where: options });
  }

  /**
   * @tested
   * @param {object} options
   * @param {string} options.agentId?
   * @param {string} options.channelId?
   * @returns {Promise<Model>}
   */
  async getAll(options) {
    return await models.AgentsChannels.findAll({ where: options });
  }

  /**
   * @param {object} options
   * @param {string} [options.agentId]
   * @param {string} [options.channelId]
   * @returns {Promise<string[]>}
   */
  async getAllChannelIds(options) {
    return (await this.getAll(options)).map(o => o.channelId);
  }

}

module.exports = {
  AgentsChannelsData,
};
