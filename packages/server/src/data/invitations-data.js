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
      const channelId = _.get(input, 'channelId');
      const email = _.get(input, 'email');
      const inviterAgentId = _.get(input, 'inviterAgentId');

      assert(_.isString(email), 'Email should be a string');
      assert(_.isString(channelId), 'Channel Id should be a string');
      assert(_.isString(inviterAgentId), 'Inviter Agent Id should be a string');

      const user = await this.users.getOne({ email });

      if (user) {
        const invitation = await this.getOne({ agentId: user.agentId });
        const agentId = _.get(invitation, 'agentId');

        assert(!!_.get(user, 'isEmailVerified'), 'Email is not verified');
        assert(!!invitation, 'User is already invited.');

        await this._addMembership({ channelId, agentId, inviterAgentId });
        return true;
      }

      const createdUser = await this._addUser(email);
      await this._addInvitation(channelId, inviterAgentId, createdUser);
      return true;
    } catch (e) {
      console.error('Invitation Err', e.message);
      return false;
    }
  }

  /**
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} inviterAgentId
   * @param {object} createdUser
   * @return {Promise<*>}
   * @protected
   */
  async _addInvitation(channelId, inviterAgentId, createdUser) {
    return this.createOne({
      channelId,
      inviterAgentId,
      agentId: createdUser.agentId
    });
  }

  /**
   * @protected
   * @param {string} email
   * @return {Promise<*>}
   */
  async _addUser(email) {
    const auth0Id = new ForetoldAuthId(email).toString();
    return this.users.createOne({
      email,
      auth0Id,
      isEmailVerified: false,
    });
  }

  /**
   * @public
   * @param {Models.ObjectID} agentId
   * @return {Promise<boolean>}
   */
  async activateAgent(agentId) {
    assert(_.isString(agentId), 'Agent Id should be a string');
    const invitations = await this._getInvitations(agentId);
    return await this._activateInvitations(invitations);
  }

  /**
   *
   * @param invitations
   * @return {Promise<boolean>}
   * @private
   */
  async _activateInvitations(invitations) {
    if (invitations.length === 0) return false;
    for (let i = 0, max = invitations.length; i < max; i++) {
      const invitation = invitations[i];
      await this._changeStatus(invitation);
      await this._addMembership(invitation);
    }
    return true;
  }

  /**
   * @param {Models.ObjectID} agentId
   * @return {Promise<Models.Invitation>}
   * @protected
   */
  async _getInvitations(agentId) {
    const filter = { agentId, status: INVITATION_STATUS.AWAITING };
    return this.getAll(filter);
  }

  /**
   * @param {Models.Invitation} invitation
   * @return {Promise<*>}
   * @protected
   */
  async _changeStatus(invitation) {
    const params = { id: invitation.id };
    const data = { status: INVITATION_STATUS.ACCEPTED };
    return this.updateOne(params, data);
  }

  /**
   * @param {Models.Invitation | {
   *   channelId: Models.ObjectID,
   *   agentId: Models.ObjectID,
   *   inviterAgentId: Models.ObjectID,
   * }} options
   * @return {Promise<*>}
   * @protected
   */
  async _addMembership(options) {
    const data = {
      channelId: options.channelId,
      agentId: options.agentId,
      inviterAgentId: options.inviterAgentId
    };
    return this.memberships.createOne(data);
  }
}

module.exports = {
  InvitationsData,
};
