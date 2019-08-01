const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('./producer');

const { getChannelLinkWithToken } = require('../../lib/urls');
const { getAgentLinkWithToken } = require('../../lib/urls');

class MemberAddedToCommunity extends Producer {
  constructor(channelMembership = {}) {
    super({});

    assert(_.isObject(channelMembership), 'Input is not an object');
    assert(!!_.get(channelMembership, 'channelId'), 'Channel ID is required.');
    assert(!!_.get(channelMembership, 'agentId'), 'Agent ID is required.');
    assert(!!_.get(channelMembership, 'inviterAgentId'), 'Inviter Agent ID is required.');

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
      const agent = await Producer.data.agents.getOne({
        id: this.channelMembership.agentId,
      });
      const inviter = await Producer.data.users.getOne({
        agentId: this.channelMembership.inviterAgentId,
      });

      assert(!!_.get(channel, 'id'), 'Channel ID is required.');
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');
      assert(!!_.get(inviter, 'id'), 'Inviter ID is required.');

      const replacements = this._getReplacements(
        inviter,
        channel,
      );

      const notification = await this._queueEmail(replacements);
      await this._assignNotification(agent, notification);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      console.log(`memberAddedToCommunity`, e.message, e);
      return false;
    }
  }

  /**
   * @protected
   * @param {object} inviter
   * @param {object} channel
   * @return {*}
   */
  _getReplacements(inviter, channel) {
    return {
      inviterAgent: {
        name: _.get(inviter, 'name') || 'Somebody',
        link: getAgentLinkWithToken(inviter),
      },
      channel: {
        name: _.get(channel, 'name') || 'Channel',
        link: getChannelLinkWithToken(channel),
      },
    }
  }
}

module.exports = {
  MemberAddedToCommunity,
};
