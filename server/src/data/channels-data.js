const _ = require('lodash');

const { ChannelMembershipsData } = require('./channel-memberships-data');
const { DataBase } = require('./data-base');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class ChannelsData extends DataBase {

  constructor() {
    super();
    this.channelMembershipsData = new ChannelMembershipsData();
  }

  /**
   * @public
   * @param {Models.User} user
   * @param {Schema.ChannelsInput} input
   * @return {Promise<Models.Channel>}
   */
  async createOne(user, input) {
    let channel = await this.models.Channel.findOne({
      where: { name: input.name },
    });
    if (channel) {
      return Promise.reject(new Error('Channel exists.'));
    }
    channel = await this.models.Channel.create({
      ...input,
      creatorId: user.agentId,
    });
    await this.channelMembershipsData.createOne(
      channel.id,
      user.agentId,
      this.models.ChannelMemberships.ROLE.ADMIN,
    );
    return channel;
  }

  /**
   * @public
   * @param {string} id
   * @param {object} input
   * @return {Promise<Models.Channel>}
   */
  async updateOne(id, input) {
    const channel = await this.models.Channel.findOne({
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
    const channel = await this.models.Channel.findOne({
      where: { id },
      include: [{ model: this.models.Agent, as: 'agents' }],
    });
    return _.get(channel, 'agents', []);
  }

  /**
   * @public
   * @param {string} id
   * @return {Promise<Model>}
   */
  async getCreatorByChannelId(id) {
    const channel = await this.models.Channel.findOne({
      where: { id },
      include: [{ model: this.models.Agent, as: 'creator' }],
    });
    return _.get(channel, 'creator');
  }

  /**
   * @public
   * @param {object} options
   * @param {number} [options.offset]
   * @param {number} [options.limit]
   * @param {string} [options.agentId]
   * @return {Promise<Models.Channel[]>}
   */
  async getAll(options = {}) {
    const offset = _.get(options, 'offset', 0);
    const limit = _.get(options, 'limit', 10);
    return await this.models.Channel.findAll({
      limit,
      offset,
      order: [['createdAt', 'DESC']],
      where: {
        id: {
          [this.models.sequelize.Op.in]: this.channelIdsLiteral(options.agentId),
        },
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
    const restrictions = 'agentId' in options ? {
      id: {
        [this.models.sequelize.Op.in]: this.channelIdsLiteral(options.agentId),
      }
    } : {};
    return await this.models.Channel.findOne({
      where: {
        [this.models.sequelize.Op.and]: [
          { id },
          restrictions
        ]
      }
    });
  }
}

module.exports = {
  ChannelsData,
};
