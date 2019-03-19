const _ = require('lodash');

const models = require("../models");

const { UsersData } = require('./users-data');

class ChannelsData {

  constructor() {
    this.usersData = new UsersData();
  }

  async channelUpdate(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  async channelCreate(root, values, options) {
    // @todo: fix
    const auth0Id = await this.usersData.getAuth0Id(options);
    const user = await this.usersData.auth0User(auth0Id);

    const input = _.get(values, 'input');
    const channel = await this.createOne(user, input);

    return channel;
  }

  async addPersonToChannel(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  async removePersonFromChannel(root, values, options) {
    console.log(root, values, options);
    return true;
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

      await models.AgentsChannels.create({
        channelId: channel.id,
        agentId: user.agentId,
      });
    }

    return channel;
  }

  /**
   * @param {string} id
   * @return {Promise<Model>}
   */
  async getAgentsByChannelId(id) {
    const channel = await models.Channel.findOne({
      where: { id },
      include: [{model: models.Agent, as: 'agents'}],
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
      include: [{model: models.Agent, as: 'creator'}],
    });
    return channel.creator;
  }

}

module.exports = {
  ChannelsData,
};
