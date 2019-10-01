const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');

const { ChannelMembershipsData } = require('./channel-memberships-data');

const { InvitationModel } = require('../models-abstract');
const { INVITATION_STATUS } = require('../enums/invitation-status');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../enums/channel-membership-type');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {InvitationModel} InvitationModel
 */
class InvitationsData extends DataBase {
  constructor() {
    super();
    this.model = new InvitationModel();
    this.channelMemberships = new ChannelMembershipsData();
  }

  /**
   * @public
   * @todo: Use transactions here.
   * @todo: Do not send an invitation to existed users.
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

      const params = { email, channelId };
      const query = {};
      const data = { email, inviterAgentId, channelId };
      return await this.upsertOne(params, query, data);

    } catch (e) {
      console.error('Invitation Err', e);
    }
    return null;
  }

  /**
   * @public
   * @todo: To add transactions.
   * @todo: To make it sync?
   * @param {Models.User} user
   * @returns {Promise<boolean>}
   */
  async transition(user) {
    try {
      /** @type {string} */
      const email = _.get(user, 'email');
      const agentId = _.get(user, 'agentId');

      assert(_.isString(email), 'Email should be a string.');
      assert(_.isString(agentId), 'Agent ID is required.');

      const invitations = await this.getAll({
        email,
        status: INVITATION_STATUS.AWAITING,
      });

      const methodCreatedBy = CHANNEL_MEMBERSHIP_TYPE.ADDED_BY_EMAIL_BY_ADMIN;

      for (let i = 0, max = invitations.length; i < max; i++) {
        const invitation = invitations[i];
        const channelId = _.get(invitation, 'channelId');
        const inviterAgentId = _.get(invitation, 'inviterAgentId');

        await this.updateOne({
          id: invitation.id,
        }, {
          status: INVITATION_STATUS.ACCEPTED,
        });

        await this.channelMemberships.upsertOne({
          agentId,
          channelId,
        }, {}, {
          agentId,
          channelId,
          inviterAgentId,
          methodCreatedBy,
        });
      }

      return true;
    } catch (e) {
      console.error('Transition Err', e);
    }
    return false;
  }
}

module.exports = {
  InvitationsData,
};
