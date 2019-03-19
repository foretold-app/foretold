const _ = require('lodash');

const models = require("../models");

const { UsersData } = require('./users-data');

class ChannelsData {

  constructor() {
    this.usersData = new UsersData();
  }

  /**
   * @param values
   * @returns {Promise<Model>}
   */
  async channelUpdate(values) {
    const id = _.get(values, 'id');
    const input = _.get(values, 'input');
    return await this.updateOne(id, input);
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
   * @param root
   * @param values
   * @param options
   * @returns {Promise<boolean>}
   */
  async addPersonToChannel(root, values, options) {
    console.log(root, values, options);
    return true;
  }

  /**
   * @param root
   * @param values
   * @param options
   * @returns {Promise<boolean>}
   */
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
   * @param {object} input
   * @return {Promise<Model>}
   */
  async updateOne(id, input) {
    return await models.Channel.update({
      input,
      where: { id },
    });
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
