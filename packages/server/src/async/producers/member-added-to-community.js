const assert = require('assert');
const _ = require('lodash');

const { getChannelLinkWithToken } = require('../../lib/urls');
const { getAgentLinkWithToken } = require('../../lib/urls');

const { Producer } = require('./producer');

class MemberAddedToCommunity extends Producer {
  constructor(channelMembership = {}, inviterAgent = {}) {
    super({});

    assert(_.isObject(channelMembership), 'Channel Membership is not an object');
    assert(_.isObject(inviterAgent), 'Inviter Agent is not an object');

    this.channelMembership = channelMembership;
    this.inviterAgent = inviterAgent;

    this.templateName = Producer.TEMPLATE_NAME.MEMBER_ADDED_TO_COMMUNITY;
  }

  /**
   * @protected
   * @param {object} inviterAgent
   * @param {object} channel
   * @return {{measurable: {name: *, link: *}}}
   */
  static _getReplacements(inviterAgent, channel) {
    return {
      channel: {
        name: _.get(channel, 'name'),
        link: getChannelLinkWithToken(channel),
      },
      inviterAgent: {
        name: _.get(inviterAgent, 'name'),
        link: getAgentLinkWithToken(inviterAgent),
      },
    }
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      const channel = await this.channelMembership.getChannel();
      assert(!!_.get(channel, 'id'), 'Channel ID is required.');

      const agent = await this.channelMembership.getAgent();
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');

      const replacements = MemberAddedToCommunity._getReplacements(
        this.inviterAgent,
        channel,
      );

      const notification = await this._queueEmail(replacements);
      await this._assignNotification(agent, notification);
    } catch (e) {
      await this._rollback();
      console.log(`stateChanged`, e.message, e);
      return false;
    }
    await this._commit();
    return true;
  }
}

module.exports = {
  MemberAddedToCommunity,
};
