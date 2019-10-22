const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');

const { ChannelMembershipModel } = require('../models-abstract');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../enums/channel-membership-type');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {ChannelMembershipModel} ChannelMembershipModel
 */
class ChannelMembershipsData extends DataBase {
  constructor() {
    super();
    this.model = new ChannelMembershipModel();
  }

  /**
   * @public
   * @param {object} options
   * @param {Models.ObjectID} [options.agentId]
   * @param {Models.ObjectID} [options.channelId]
   * @returns {Promise<string[]>}
   */
  async getAllChannelIds(options) {
    return (await this.getAll(options)).map((o) => o.channelId);
  }

  /**
   * @param {object} options
   * @param {Models.ObjectID} options.channelId
   * @param {object} options.agentId
   * @return {Promise<Models.ChannelMemberships>}
   */
  async join(options) {
    const data = {
      channelId: options.channelId,
      agentId: options.agentId,
      methodCreatedBy: CHANNEL_MEMBERSHIP_TYPE.AGENT_JOINED_DIRECTLY,
    };
    return this.createOne(data);
  }

  /**
   * @public
   * @param {object} options
   * @return {Promise<Models.ChannelMemberships|null>}
   */
  async leave(options) {
    return this.deleteOne({
      channelId: options.channelId,
      agentId: options.agentId,
    });
  }

  /**
   * @public
   * @param {object} options.agentId
   * @param {object} options.channelId
   * @return {Promise<string>}
   */
  async getOneOnlyRole(options) {
    assert(!!_.get(options, 'channelId'),
      'getOneOnlyRole::channelId is required.');

    if (!_.get(options, 'agentId')) {
      return ChannelMembershipModel.ROLES.NONE;
    }

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
    return this.getAll({
      ...options,
      role: ChannelMembershipModel.ROLES.ADMIN,
    });
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }
}

module.exports = {
  ChannelMembershipsData,
};
