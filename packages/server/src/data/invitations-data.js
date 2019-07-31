const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');
const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');

const { InvitationModel } = require('../models-abstract');

const { ForetoldAuthId } = require('../models/classes/foretold-auth-id');
const { INVITATION_STATUS } = require('../models/enums/invitation-status');

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
      assert(_.isString(input.email), 'Email should be a string');
      assert(_.isString(input.channelId), 'Channel Id should be a string');
      assert(_.isString(input.inviterAgentId), 'Inviter Agent Id should be a string');

      const user = await this.users.getOne({ email: input.email });

      if (user) {
        assert(!!_.get(user, 'isEmailVerified'), 'Email is not verified');

        const invitation = await this.getOne({ agentId: user.agentId });
        assert(!!invitation, 'User is already invited.');

        await this.memberships.createOne2(
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

  /**
   * @public
   * @param {Models.ObjectID} agentId
   * @return {Promise<boolean>}
   */
  async activateAgent(agentId) {
    assert(_.isString(agentId), 'Agent Id should be a string');

    const invitation = await this._getInvitation(agentId);
    if (!invitation) return false;

    await this._changeStatus(invitation);
    await this._addMembership(invitation);

    return true;
  }

  /**
   * @param {Models.ObjectID} agentId
   * @return {Promise<Models.Invitation>}
   * @protected
   */
  async _getInvitation(agentId) {
    return this.getOne({
      agentId,
      status: INVITATION_STATUS.AWAITING,
    });
  }

  /**
   * @param {Models.Invitation} invitation
   * @return {Promise<*>}
   * @protected
   */
  async _changeStatus(invitation) {
    return this.updateOne({
      id: invitation.id,
    }, {
      status: INVITATION_STATUS.ACCEPTED,
    });
  }

  /**
   * @param {Models.Invitation} invitation
   * @return {Promise<*>}
   * @protected
   */
  async _addMembership(invitation) {
    return this.memberships.createOne2(
      invitation.channelId,
      invitation.agentId,
      invitation.inviterAgentId
    );
  }
}

module.exports = {
  InvitationsData,
};
