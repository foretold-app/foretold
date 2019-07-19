const _ = require('lodash');

const { ChannelMembershipsData } = require('./channel-memberships-data');
const { DataBase } = require('./data-base');

const { ChannelModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class ChannelsData extends DataBase {

  constructor() {
    super();
    this.channelMembershipsData = new ChannelMembershipsData();
    this.ChannelModel = new ChannelModel();
    this.model = this.ChannelModel;
  }

  /**
   * @todo: fix interface (data, options)
   * @public
   * @param {Models.Agent} agent
   * @param {Schema.ChannelsInput} input
   * @return {Promise<Models.Channel>}
   */
  async createOne(agent, input) {
    let channel = await this.models.Channel.findOne({
      where: { name: input.name },
    });
    if (channel) {
      return Promise.reject(new Error('Channel exists.'));
    }
    channel = await this.models.Channel.create({
      ...input,
      creatorId: agent.id,
    });
    await this.channelMembershipsData.createOne(
      channel.id,
      agent.id,
      this.models.ChannelMemberships.ROLE.ADMIN,
    );
    return channel;
  }

  /**
   * @todo: fix interface (params, data, options)
   * @public
   * @param {Models.ObjectID} id
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
   * @todo: rework
   * @public
   * @param {Models.ObjectID} id
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
   * @todo: rework
   * @public
   * @param {Models.ObjectID} id
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
   * @todo: fix interface (filter, pagination, options)
   * @todo: move down (model abstraction)
   * @public
   * @param {object} [filter]
   * @param {number} [filter.offset]
   * @param {number} [filter.limit]
   * @param {Models.ObjectID} [filter.agentId]
   * @param {object} [_pagination]
   * @param {object} [_options]
   * @return {Promise<Models.Channel[]>}
   */
  async getAll(filter = {}, _pagination = {}, _options = {}) {
    const offset = _.get(filter, 'offset', 0);
    const limit = _.get(filter, 'limit', 10);
    return this.models.Channel.findAll({
      limit,
      offset,
      order: [['createdAt', 'DESC']],
      where: {
        id: {
          [this.model.Op.in]: this.ChannelModel._channelIdsLiteral(filter.agentId),
        },
      },
    });
  }

  /**
   * @todo: fix interface (params, query, options)
   * @todo: move down (model abstraction)
   * @public
   * @param {Models.ObjectID} id
   * @param {object} options
   * @param {number} [options.offset]
   * @param {number} [options.limit]
   * @param {Models.ObjectID} [options.agentId]
   * @return {Promise<Models.Channel>}
   */
  async getOne(id, options = {}) {
    const restrictions = 'agentId' in options ? {
      id: {
        [this.model.Op.in]: this.ChannelModel._channelIdsLiteral(options.agentId),
      },
    } : {};
    return this.models.Channel.findOne({
      where: {
        [this.model.Op.and]: [
          { id },
          restrictions,
        ],
      },
    });
  }
}

module.exports = {
  ChannelsData,
};
