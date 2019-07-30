const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');
const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');

const { InvitationModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {InvitationModel} InvitationModel
 */
class InvitationsData extends DataBase {

  constructor() {
    super();
    this.InvitationModel = new InvitationModel();
    this.model = this.InvitationModel;
    this.users = new UsersData();
    this.agents = new AgentsData();
    this.memberships = new ChannelMembershipsData();
  }

  /**
   * @public
   * @param {object} datas
   * @param {string} datas.email
   * @param {string} datas.channelId
   * @param {string} datas.inviterAgentId
   * @return {Promise<*>}
   */
  async invite(datas) {
    assert(_.isString(datas.email), 'Email should be an string');
    assert(_.isString(datas.channelId), 'Channel Id should be an string');
    assert(_.isString(datas.inviterAgentId), 'Inviter Agent Id should be an string');

    const user = await this.users.getOne({ email: datas.email });
    if (user) {
      await this.memberships.createOne(datas.channelId, user.agentId, datas.inviterAgentId);
      return true;
    }

  }

}

module.exports = {
  InvitationsData,
};
