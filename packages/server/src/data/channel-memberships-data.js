const _ = require('lodash');

const { DataBase } = require('./data-base');

const { ChannelMembershipModel } = require('../models-abstract');

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
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @param {Models.ObjectID} inviterAgentId
   * @param {string} role
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async createOne(channelId, agentId, inviterAgentId, role) {
    await this.validate({ channelId, agentId });
    const channelMembership =
      await this.models.ChannelMemberships.findOne({ where: { channelId, agentId } }) ||
      await this.models.ChannelMemberships.create({ channelId, agentId, inviterAgentId, role });
    return channelMembership;
  }

  /**
   * @todo: fix interface (params, query, options)
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @param {string} role
   * @returns {Promise<Models.ChannelMemberships>}
   */
  async updateOne(channelId, agentId, role) {
    await this.validate({ channelId, agentId });
    return this.ChannelMembershipModel.updateOne(
      channelId,
      agentId,
      role,
    );
  }

  /**
   * @todo: fix interface (params, options)
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} agentId
   * @returns {Promise<Models.ChannelMemberships | null>}
   */
  async deleteOne(channelId, agentId) {
    await this.validate({ channelId, agentId });
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
  async validate({ channelId, agentId }) {
    if (!await this.models.Channel.findByPk(channelId)) {
      return Promise.reject(new Error(`Channel "${ channelId }" is not found.`));
    }
    if (!await this.models.Agent.findByPk(agentId)) {
      return Promise.reject(new Error(`Agent "${ agentId }" is not found.`));
    }
    return true;
  }

  /**
   *
   * @todo: Call "getOne()" of "model" layer.
   * @todo: Pass "id", "restrictions" and "filter" params
   * @todo: right into model call.
   * @todo: Then do the same everywhere on data layer.
   *
   * @todo: fix interface (params, query, options)
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
   * @param {object} options
   * @param {Models.ObjectID} [options.agentId]
   * @param {Models.ObjectID} [options.channelId]
   * @returns {Promise<Models.ChannelMemberships[]>}
   */
  async getAll(options) {
    return this.models.ChannelMemberships.findAll({ where: options });
  }

  /**
   * @param {object} options
   * @param {Models.ObjectID} [options.agentId]
   * @param {Models.ObjectID} [options.channelId]
   * @returns {Promise<string[]>}
   */
  async getAllChannelIds(options) {
    return (await this.getAll(options)).map(o => o.channelId);
  }

  /**
   * @todo: createOneAs?
   * @param {object} options
   * @return {Promise<Models.ChannelMemberships>}
   */
  async join(options) {
    const role = ChannelMembershipModel.ROLES.VIEWER;
    return this.createOne(
      options.channelId,
      options.agentId,
      null,
      role,
    );
  }

  /**
   * @todo: updateOneAs?
   * @param {object} options
   * @return {Promise<Models.ChannelMemberships|null>}
   */
  async leave(options) {
    return this.deleteOne(
      options.channelId,
      options.agentId,
    );
  }

  /**
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
   * @param {object} options
   * @return {Promise<Models.ChannelMemberships[]>}
   */
  async getAllOnlyAdmins(options) {
    return this.getAll({
      ...options,
      role: ChannelMembershipModel.ROLES.ADMIN,
    });
  }
}

module.exports = {
  ChannelMembershipsData,
};
