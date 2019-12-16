const assert = require('assert');
const _ = require('lodash');

const { getChannelLinkWithToken } = require('../../../lib/client-urls');
const { getAgentLinkWithToken } = require('../../../lib/client-urls');
const logger = require('../../../lib/log');

const { Producer } = require('../producer');
const { ProducerNotifications } = require('./producer-notifications');

const log = logger.module('notifications/member-to-community');

/**
 * @abstract
 */
class MemberToCommunity extends ProducerNotifications {
  /**
   * @param {Models.ChannelMemberships} channelMembership
   */
  constructor(channelMembership) {
    super({});

    assert(_.isObject(channelMembership),
      'Channel Membership is not an object');
    assert(!!_.get(channelMembership,
      'channelId'), 'Channel ID is required.');
    assert(!!_.get(channelMembership,
      'agentId'), 'Agent ID is required.');

    this.channelMembership = channelMembership;
    this.templateName = Producer.TEMPLATE_NAME.MEMBER_ADDED_TO_COMMUNITY;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    if (!this.channelMembership.inviterAgentId) {
      log.trace(this.constructor.name, 'There is no "inviterAgentId"');
      return true;
    }

    try {
      /** @type {Models.Channel} */
      const channel = await this.channels.getOne({
        id: this.channelMembership.channelId,
      });
      /** @type {Models.Agent} */
      const agent = await this.agents.getOne({
        id: this.channelMembership.agentId,
      });
      /** @type {Models.Agent} */
      const inviter = await this.users.getOne({
        agentId: this.channelMembership.inviterAgentId,
      });

      assert(!!_.get(channel, 'id'), 'Channel ID is required.');
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');
      assert(!!_.get(inviter, 'id'), 'Inviter ID is required.');

      const replacements = await this._getReplacements(
        inviter,
        channel,
      );

      /** @type {Models.Notification} */
      const notification = await this._queueEmail(replacements);
      await this._assignNotification(agent, notification);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      log.trace(this.constructor.name, e.message, e);
      return false;
    }
  }

  /**
   * @protected
   * @param {Models.Agent} inviter
   * @param {Models.Channel} channel
   * @return {object}
   */
  async _getReplacements(inviter, channel) {
    return {
      inviterAgent: {
        name: (await _.get(inviter, 'name')) || 'Somebody',
        link: getAgentLinkWithToken(inviter),
      },
      channel: {
        name: _.get(channel, 'name') || 'Channel',
        link: getChannelLinkWithToken(channel),
      },
    };
  }
}

module.exports = {
  MemberToCommunity,
};
