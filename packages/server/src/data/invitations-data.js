const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');
const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');

const { InvitationModel } = require('../models-abstract');

const { ForetoldAuthId } = require('../models/classes/foretold-auth-id');

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
   * @todo: Use transactions here.
   * @param {object} input
   * @param {string} input.email
   * @param {string} input.channelId
   * @param {string} input.inviterAgentId
   * @return {Promise<Object>}
   */
  async invite(input) {
    try {
      assert(_.isString(input.email), 'Email should be an string');
      assert(_.isString(input.channelId), 'Channel Id should be an string');
      assert(_.isString(input.inviterAgentId), 'Inviter Agent Id should be an string');

      const user = await this.users.getOne({ email: input.email });

      if (user) {
        assert(!!_.get(user, 'isEmailVerified'), 'Email is not verified.');
        await this.memberships.createOne(
          input.channelId,
          user.agentId,
          input.inviterAgentId
        );
        return true;
      }

      const createdUser = await this.users.createOne({
        email: input.email,
        auth0Id: new ForetoldAuthId(input.email).toString(),
        isEmailVerified: false,
      });

      const invitation = await this.createOne({
        agentId: createdUser.agentId,
        channelId: input.channelId,
        inviterAgentId: input.inviterAgentId,
      });

      return true;
    } catch (e) {
      console.error('Invitation Err', e.message);
      return false;
    }
  }

}

module.exports = {
  InvitationsData,
};
