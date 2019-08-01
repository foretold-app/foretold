const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');
const { UsersData } = require('./users-data');
const { AgentsData } = require('./agents-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');
const { TokensData } = require('./tokens-data');

const { InvitationModel } = require('../models-abstract');

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
    this.tokens = new TokensData();
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

      const token = await this.tokens.generateToken();
      await this._addInvitation(channelId, inviterAgentId, token, email);

      return true;
    } catch (e) {
      console.error('Invitation Err', e.message);
      return false;
    }
  }

  /**
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} inviterAgentId
   * @param {string} token
   * @param {string} email
   * @return {Promise<*>}
   * @protected
   */
  async _addInvitation(channelId, inviterAgentId, token, email) {
    return this.createOne({
      channelId,
      inviterAgentId,
      token,
      email,
    });
  }

  /**
   * @public
   * @param {string} token
   * @param {Models.User} user
   * @return {Promise<boolean>}
   */
  async activate(token, user) {
    assert(_.isString(token), 'Token should be a string');
    const invitation = await this._getInvitation(token);
    await this._changeStatus(invitation);
    await this._addMembership(invitation, user);
    return true;
  }

  /**
   * @param {string} token
   * @return {Promise<Models.Invitation>}
   * @protected
   */
  async _getInvitation(token) {
    const params = { token, status: INVITATION_STATUS.AWAITING };
    return this.getOne(params);
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
   * @param {Models.Invitation} invitation
   * @param {Models.User} user
   * @return {Promise<Models.ChannelMembership>}
   * @protected
   */
  async _addMembership(invitation, user) {
    const data = {
      channelId: invitation.channelId,
      agentId: user.agentId,
      inviterAgentId: invitation.inviterAgentId
    };
    return this.memberships.createOne(data);
  }
}

module.exports = {
  InvitationsData,
};
