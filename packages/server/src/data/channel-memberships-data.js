const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');

const { ChannelMembershipModel } = require('./models');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../enums');

const { Data } = require('./classes');
const { Filter } = require('./classes');
const { Params } = require('./classes');
const { Pagination } = require('./classes');
const { Options } = require('./classes');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {ChannelMembershipModel} model
 */
class ChannelMembershipsData extends DataBase {
  constructor() {
    super();
    this.model = new ChannelMembershipModel();
  }

  /**
   * @public
   * @param {object} filter
   * @param {Defs.AgentID} [filter.agentId]
   * @param {Defs.ChannelID} [filter.channelId]
   * @returns {Promise<string[]>}
   */
  async getAllChannelIds(filter) {
    return (await this.getAll(filter)).map((o) => o.channelId);
  }

  /**
   * @param {object} options
   * @param {Defs.ChannelID} options.channelId
   * @param {Defs.AgentID} options.agentId
   * @return {Promise<Models.ChannelMemberships>}
   */
  async join(options) {
    assert(!!_.get(options, 'agentId'),
      'join::agentId is required.');
    assert(!!_.get(options, 'channelId'),
      'join::channelId is required.');

    const data = new Data({
      channelId: options.channelId,
      agentId: options.agentId,
      methodCreatedBy: CHANNEL_MEMBERSHIP_TYPE.AGENT_JOINED_DIRECTLY,
    });
    return this.createOne(data);
  }

  /**
   * @public
   * @param {object} options
   * @param {Defs.AgentID} options.agentId
   * @param {Defs.ChannelID} options.channelId
   * @return {Promise<Models.ChannelMemberships|null>}
   */
  async leave(options) {
    assert(!!_.get(options, 'agentId'),
      'leave::agentId is required.');
    assert(!!_.get(options, 'channelId'),
      'leave::channelId is required.');

    return this.deleteOne(new Params({
      channelId: options.channelId,
      agentId: options.agentId,
    }));
  }

  /**
   * @public
   * @param {Defs.AgentID} agentId
   * @param {Defs.ChannelID} channelId
   * @param {boolean} isVerified
   * @return {Promise<boolean>}
   */
  async verify(channelId, agentId, isVerified) {
    assert(!!agentId, 'verify::agentId is required.');
    assert(!!channelId, 'verify::channelId is required.');
    assert(_.isBoolean(isVerified), 'isVerified should be boolean.');

    try {
      const params = new Params({ agentId, channelId });
      const data = new Data({ isVerified });
      const result = await this.updateOne(params, data);
      return !!result;
    } catch (e) {
      this.log.error(e.message);
    }
    return false;
  }

  /**
   * @public
   * @param {object} params
   * @param {Defs.AgentID} params.agentId
   * @param {Defs.ChannelID} params.channelId
   * @return {Promise<string>}
   */
  async getOneOnlyRole(params) {
    assert(!!_.get(params, 'channelId'),
      'getOneOnlyRole::channelId is required.');

    if (!_.get(params, 'agentId')) {
      return ChannelMembershipModel.ROLES.NONE;
    }

    const channelMembership = await this.getOne(params);
    return _.get(
      channelMembership,
      'role',
      ChannelMembershipModel.ROLES.NONE,
    );
  }

  /**
   * @public
   * @param {Defs.ChannelID} channelId
   * @return {Promise<Models.ChannelMemberships[]>}
   */
  async getAllOnlyAdmins(channelId) {
    const role = ChannelMembershipModel.ROLES.ADMIN;
    const filter = new Filter({ channelId, role });
    const pagination = new Pagination();
    const options = new Options({ raw: true });
    return this.getAll(filter, pagination, options);
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelRestrictions}
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
