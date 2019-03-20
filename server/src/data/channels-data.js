const _ = require('lodash');

const models = require("../models");

const { UsersData } = require('./users-data');
const { AgentsChannelsData } = require('./agents-channels-data');

class ChannelsData {

  constructor() {
    this.usersData = new UsersData();
    this.agentsChannelsData = new AgentsChannelsData();
  }

  /**
   * @param values
   * @param options
   * @returns {Promise<Model>}
   */
  async channelCreate(values, options) {
    // @todo:
    const auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(auth0Id);

    const input = _.get(values, 'input');
    return await this.createOne(user, input);
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
    await channel.update(input);
    return channel;
  }

  /**
   * @param {string} id
   * @return {Promise<Model>}
   */
  async getAgentsByChannelId(id) {
    const channel = await models.Channel.findOne({
      where: { id },
      include: [{ model: models.Agent, as: 'agents' }],
    });
    return channel.agents;
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
    return channel.creator;
  }

}

module.exports = {
  ChannelsData,
};
