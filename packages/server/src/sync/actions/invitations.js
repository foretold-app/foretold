const _ = require('lodash');
const assert = require('assert');

const { InvitationsData } = require('../../data');
const { ChannelMembershipsData } = require('../../data');
const logger = require('../../lib/log');

const { INVITATION_STATUS } = require('../../enums');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../../enums');

const log = logger.module('sync/actions/user-updater');

class Invitations {
  constructor() {
    this.invitations = new InvitationsData();
    this.channelMemberships = new ChannelMembershipsData();
  }

  /**
   * @public
   * @todo: To add transactions.
   * @param {Models.User} user
   * @returns {Promise<boolean>}
   */
  async transition(user) {
    try {
      /** @type {string} */
      const email = _.get(user, 'email', null);
      const agentId = _.get(user, 'agentId', null);

      assert(_.isString(email), 'Email should be a string.');
      assert(_.isString(agentId), 'Agent ID is required.');

      const invitations = await this.invitations.getAll({
        email,
        status: INVITATION_STATUS.AWAITING,
      });

      const methodCreatedBy = CHANNEL_MEMBERSHIP_TYPE.ADDED_BY_EMAIL_BY_ADMIN;

      for (let i = 0, max = invitations.length; i < max; i++) {
        const invitation = invitations[i];

        const channelId = _.get(invitation, 'channelId', null);
        const inviterAgentId = _.get(invitation, 'inviterAgentId', null);

        await this.invitations.updateOne({
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
      log.trace('Transition Err', e.message);
    }
    return false;
  }
}

module.exports = {
  Invitations,
};
