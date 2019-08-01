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
    this.defaultRestrictions = { channelIdAsId: true };
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
      null,
      this.models.ChannelMemberships.ROLE.ADMIN,
    );
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
}

module.exports = {
  ChannelsData,
};
