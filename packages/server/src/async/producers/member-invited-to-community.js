const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('./producer');

const { getInvitationLinkWithInvitationToken } = require('../../lib/urls');
const { getAgentLink } = require('../../lib/urls');

class MemberInvitedToCommunity extends Producer {
  constructor(invitation = {}) {
    super({});

    assert(_.isObject(invitation), 'Input is not an object');
    assert(!!_.get(invitation, 'token'), 'Token is required.');
    assert(!!_.get(invitation, 'inviterAgentId'), 'Inviter Agent ID is required.');

    this.invitation = invitation;
    this.templateName = Producer.TEMPLATE_NAME.MEMBER_INVITED_TO_COMMUNITY;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      const inviter = await Producer.data.users.getOne({
        agentId: this.invitation.inviterAgentId,
      });
      assert(!!_.get(inviter, 'id'), 'Inviter ID is required.');

      const replacements = this._getReplacements(
        inviter,
        this.invitation,
      );

      const notification = await this._queueEmail(replacements);
      await this._assignNotification(null, notification);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      console.log(`MemberInvitedToCommunity`, e.message, e);
      return false;
    }
  }

  /**
   * @protected
   * @param {Models.Agent} inviter
   * @param {Models.Invitation} invitation
   * @return {*}
   */
  _getReplacements(inviter, invitation) {
    return {
      inviterAgent: {
        name: _.get(inviter, 'name') || 'Somebody',
        link: getAgentLink(inviter),
      },
      invitation: {
        link: getInvitationLinkWithInvitationToken(invitation),
      },
    }
  }
}

module.exports = {
  MemberInvitedToCommunity,
};
