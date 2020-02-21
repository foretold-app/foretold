const _ = require('lodash');
const assert = require('assert');

const { InvitationsData } = require('../../data');
const { ChannelMembershipsData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Filter } = require('../../data/classes');

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
   * @param {Defs.User} user
   * @returns {Promise<boolean>}
   */
  async transition(user) {
    try {
      /** @type {string} */
      const email = _.get(user, 'email', null);
      const agentId = _.get(user, 'agentId', null);

      assert(_.isString(email), 'Email should be a string.');
      assert(_.isString(agentId), 'Agent ID is required.');

      const invitations = await this.invitations.getAll(new Filter({
        email,
        status: INVITATION_STATUS.AWAITING,
      }));

      const methodCreatedBy = CHANNEL_MEMBERSHIP_TYPE.ADDED_BY_EMAIL_BY_ADMIN;

      for (let i = 0, max = invitations.length; i < max; i++) {
        const invitation = invitations[i];

        const channelId = _.get(invitation, 'channelId', null);
        const inviterAgentId = _.get(invitation, 'inviterAgentId', null);

        await this.invitations.updateOne(new Params({
          id: invitation.id,
        }), new Data({
          status: INVITATION_STATUS.ACCEPTED,
        }));

        await this.channelMemberships.upsertOne(new Params({
          agentId,
          channelId,
        }), new Query(), new Data({
          agentId,
          channelId,
          inviterAgentId,
          methodCreatedBy,
        }));
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
