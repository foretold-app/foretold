const _ = require('lodash');

const { DataBase } = require('./data-base');

const { ChannelMembershipModel } = require('../models-abstract');
const { CHANNEL_MEMBERSHIP_ROLES } = require('../models/enums/channel-membership-roles');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {ChannelMembershipModel} ChannelMembershipModel
 */
class ChannelMembershipsData extends DataBase {

  constructor() {
    super();
    this.ChannelMembershipModel = new ChannelMembershipModel();
    this.model = this.ChannelMembershipModel;
  }

  /**
   * @todo: fix interface (data, options)
   * @public
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} [inviterAgentId]
   * @param {string} [role]
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async createOne2(
    channelId,
    agentId,
    inviterAgentId = null,
    role = CHANNEL_MEMBERSHIP_ROLES.VIEWER,
  ) {
    await this._validate({ channelId, agentId });
    return await this.models.ChannelMemberships.findOne({
        where: {
          channelId,
          agentId
        }
      }) || await this.models.ChannelMemberships.create({
        channelId,
        agentId,
        inviterAgentId,
        role
      });
  }

  /**
   * @todo: fix interface (params, query, options)
   * @public
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @param {string} role
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async updateOne2(channelId, agentId, role) {
    await this._validate({ channelId, agentId });
    return this.ChannelMembershipModel.updateOne(
      channelId,
      agentId,
      role,
    );
  }

  /**
   * @todo: fix interface (params, options)
   * @public
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @returns {Promise<Models.ChannelMemberships | null>}
   */
  async deleteOne2(channelId, agentId) {
    await this._validate({ channelId, agentId });
    return this.ChannelMembershipModel.deleteOne(
      channelId,
      agentId,
    );
  }

  /**
   * @protected
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @return {Promise<*>}
   */
  async _validate({ channelId, agentId }) {
    if (!await this.models.Channel.findByPk(channelId)) {
      return Promise.reject(new Error(`Channel "${channelId}" is not found.`));
    }
    if (!await this.models.Agent.findByPk(agentId)) {
      return Promise.reject(new Error(`Agent "${agentId}" is not found.`));
    }
    return true;
  }

  /**
   * @todo: fix interface (params, query, options)
   * @public
   * @param {object} options
   * @param {Models.ObjectID} [options.agentId]
   * @param {Models.ObjectID} [options.channelId]
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async getOne(options) {
    return this.models.ChannelMemberships.findOne({ where: options });
  }

  /**
   * @todo: fix interface (filter, pagination, options)
   * @public
   * @param {object} options
   * @param {Models.ObjectID} [options.agentId]
   * @param {Models.ObjectID} [options.channelId]
   * @param {string} [options.role]
   * @returns {Promise<Models.ChannelMemberships[]>}
   */
  async getAll2(options) {
    return this.models.ChannelMemberships.findAll({ where: options });
  }

  /**
   * @public
   * @param {object} options
   * @param {Models.ObjectID} [options.agentId]
   * @param {Models.ObjectID} [options.channelId]
   * @returns {Promise<string[]>}
   */
  async getAllChannelIds(options) {
    return (await this.getAll2(options)).map(o => o.channelId);
  }

  /**
   * @todo: createOneAs?
   * @param {object} options
   * @return {Promise<Models.ChannelMemberships>}
   */
  async join(options) {
    return this.createOne2(
      options.channelId,
      options.agentId,
    );
  }

  /**
   * @public
   * @param {object} options
   * @return {Promise<Models.ChannelMemberships|null>}
   */
  async leave(options) {
    return this.deleteOne2(
      options.channelId,
      options.agentId,
    );
  }

  /**
   * @public
   * @param {object} options
   * @return {Promise<string>}
   */
  async getOneOnlyRole(options) {
    const channelMembership = await this.getOne(options);
    return _.get(
      channelMembership,
      'role',
      ChannelMembershipModel.ROLES.NONE,
    );
  }

  /**
   * @public
   * @param {object} options
   * @return {Promise<Models.ChannelMemberships[]>}
   */
  async getAllOnlyAdmins(options) {
    return this.getAll2({
      ...options,
      role: ChannelMembershipModel.ROLES.ADMIN,
    });
  }
}

module.exports = {
  ChannelMembershipsData,
};
