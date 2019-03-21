const _ = require('lodash');

const models = require('../models');

const { AgentsChannelsData } = require('./agents-channels-data');

class ChannelsData {

  constructor() {
    this.agentsChannelsData = new AgentsChannelsData();
  }

  /**
   * @param {Model} user
   * @param {object} input
   * @return {Promise<Model>}
   */
  async createOne(user, input) {
    let channel = await models.Channel.findOne({
      where: { name: input.name },
    });

    if (!channel) {
      channel = await models.Channel.create({
        ...input,
        creatorId: user.agentId,
      });
      await this.agentsChannelsData.createOne(channel.id, user.agentId);
    }
    return channel;
  }

  /**
   * @param {string} id
   * @param {object} input
   * @return {Promise<Model>}
   */
  async updateOne(id, input) {
    const channel = await models.Channel.findOne({
      where: { id },
    });
    if (channel) await channel.update(input);
    return channel;
  }

  /**
   * @param {string} id
   * @return {Promise<Model[]>}
   */
  async getAgentsByChannelId(id) {
    const channel = await models.Channel.findOne({
      where: { id },
      include: [{ model: models.Agent, as: 'agents' }],
    });
    return _.get(channel, 'agents', []);
  }

  /**
   * @param {string} id
   * @return {Promise<Model>}
   */
  async getCreatorByChannelId(id) {
    const channel = await models.Channel.findOne({
      where: { id },
      include: [{ model: models.Agent, as: 'creator' }],
    });
    return _.get(channel, 'creator');
  }

}

module.exports = {
  ChannelsData,
};
