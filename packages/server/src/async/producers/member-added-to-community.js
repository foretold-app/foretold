const assert = require('assert');
const _ = require('lodash');

const { getChannelLinkWithToken } = require('../../lib/urls');
const { getAgentLinkWithToken } = require('../../lib/urls');

const { Producer } = require('./producer');

class MemberAddedToCommunity extends Producer {
  constructor(channelMembership = {}) {
    super({});
    assert(_.isObject(channelMembership), 'Channel Membership is not an object');
    this.channelMembership = channelMembership;
    this.templateName = Producer.TEMPLATE_NAME.MEMBER_ADDED_TO_COMMUNITY;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      const channel = await Producer.data.channels.getOne({
        id: this.channelMembership.channelId,
      });
      assert(!!_.get(channel, 'id'), 'Channel ID is required.');

      const agent = await Producer.data.agents.getOne({
        id: this.channelMembership.agentId,
      });
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');

      const inviter = await Producer.data.users.getOne({
        agentId: this.channelMembership.inviterAgentId,
      });
      assert(!!_.get(inviter, 'id'), 'Inviter ID is required.');

      const replacements = MemberAddedToCommunity._getReplacements(
        inviter,
        channel,
      );

      const notification = await this._queueEmail(replacements);
      await this._assignNotification(agent, notification);
    } catch (e) {
      await this._rollback();
      console.log(`memberAddedToCommunity`, e.message, e);
      return false;
    }
    await this._commit();
    return true;
  }

  /**
   * @protected
   * @param {object} inviter
   * @param {object} channel
   * @return {*}
   */
  static _getReplacements(inviter, channel) {
    return {
      inviterAgent: {
        name: _.get(inviter, 'name'),
        link: getAgentLinkWithToken(inviter),
      },
      channel: {
        name: _.get(channel, 'name'),
        link: getChannelLinkWithToken(channel),
      },
    }
  }
}

module.exports = {
  MemberAddedToCommunity,
};
