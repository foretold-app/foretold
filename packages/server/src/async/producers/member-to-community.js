const assert = require('assert');
const _ = require('lodash');

const { getChannelLinkWithToken } = require('../../lib/urls');
const { getAgentLinkWithToken } = require('../../lib/urls');

const { Producer } = require('./producer');

class MemberToCommunity extends Producer {
  constructor(input = {}) {
    super({});

    assert(_.isObject(input), 'Input is not an object');
    assert(!!_.get(input, 'channelId'), 'Channel ID is required.');
    assert(!!_.get(input, 'agentId'), 'Agent ID is required.');
    assert(!!_.get(input, 'inviterAgentId'), 'Inviter Agent ID is required.');

    this.input = input;
    this.templateName = Producer.TEMPLATE_NAME.MEMBER_ADDED_TO_COMMUNITY;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      const channel = await Producer.data.channels.getOne({
        id: this.input.channelId,
      });
      const agent = await Producer.data.agents.getOne({
        id: this.input.agentId,
      });
      const inviter = await Producer.data.users.getOne({
        agentId: this.input.inviterAgentId,
      });

      assert(!!_.get(channel, 'id'), 'Channel ID is required.');
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');
      assert(!!_.get(inviter, 'id'), 'Inviter ID is required.');

      const replacements = MemberToCommunity._getReplacements(
        inviter,
        channel,
      );

      const notification = await this._queueEmail(replacements);
      await this._assignAgentToNotification(agent, notification);

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
  static _getReplacements(inviter, channel) {
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
  MemberToCommunity,
};
