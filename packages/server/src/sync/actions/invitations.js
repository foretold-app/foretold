const data = require('../../data');

const { INVITATION_STATUS } = require('../../enums/invitation-status');
const {
  CHANNEL_MEMBERSHIP_TYPE,
} = require('../../enums/channel-membership-type');

class Invitations {
  constructor() {
    this.data = data;
    this.invitations = this.data.invitations;
    this.channelMemberships = this.data.channelMemberships;
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
      const email = _.get(user, 'email');
      const agentId = _.get(user, 'agentId');

      assert(_.isString(email), 'Email should be a string.');
      assert(_.isString(agentId), 'Agent ID is required.');

      const invitations = await this.invitations.getAll({
        email,
        status: INVITATION_STATUS.AWAITING,
      });

      const methodCreatedBy = CHANNEL_MEMBERSHIP_TYPE.ADDED_BY_EMAIL_BY_ADMIN;

      for (let i = 0, max = invitations.length; i < max; i++) {
        const invitation = invitations[i];
        const channelId = _.get(invitation, 'channelId');
        const inviterAgentId = _.get(invitation, 'inviterAgentId');

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
      console.error('Transition Err', e);
    }
    return false;
  }
}

module.exports = {
  Invitations,
};
