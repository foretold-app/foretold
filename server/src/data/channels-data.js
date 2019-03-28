const _ = require('lodash');

const models = require('../models');

const { AgentsChannelsData } = require('./agents-channels-data');
const { DataBase } = require('./data-base');

class ChannelsData extends DataBase {

  constructor() {
    super();
    this.agentsChannelsData = new AgentsChannelsData();
  }

  /**
   * @public
   * @param {Models.User} user
   * @param {Schema.ChannelsInput} input
   * @return {Promise<Models.Channel>}
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
      await this.agentsChannelsData.createOne(
        channel.id,
        user.agentId, [
          models.AgentsChannels.ROLES.admin,
          models.AgentsChannels.ROLES.viewer
        ]
      );
    }
    return channel;
  }

  /**
   * @public
   * @param {string} id
   * @param {object} input
   * @return {Promise<Models.Channel>}
   */
  async updateOne(id, input) {
    const channel = await models.Channel.findOne({
      where: { id },
    });
    if (channel) await channel.update(input);
    return channel;
  }

  /**
   * @public
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
   * @public
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

  /**
   * @public
   * @param {object} options
   * @param {number} [options.offset]
   * @param {number} [options.limit]
   * @param {object} [options.agentId]
   * @return {Promise<Models.Channel[]>}
   */
  async getAll(options = {}) {
    const offset = _.get(options, 'offset', 0);
    const limit = _.get(options, 'limit', 10);
    return await models.Channel.findAll({
      limit,
      offset,
      order: [['createdAt', 'DESC']],
      where: {
        id: { $in: this.channelIdsLiteral(options.agentId) },
      }
    });
  }

  /**
   * @public
   * @param {string} id
   * @param {object} options
   * @param {number} [options.offset]
   * @param {number} [options.limit]
   * @param {string} [options.agentId]
   * @return {Promise<Models.Channel>}
   */
  async getOne(id, options = {}) {
    return await models.Channel.findOne({
      where: {
        $and: [
          { id },
          { id: { $in: this.channelIdsLiteral(options.agentId) } },
        ]
      }
    });
  }
}

module.exports = {
  ChannelsData,
};
