const assert = require('assert');
const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../../../models/enums/channel-membership-type');

const { Producer } = require('../producer');

class MemberJoinedToCommunity extends ProducerFeedItems {

  /**
   * @param {Models.ChannelMembership} channelMembership
   */
  constructor(channelMembership) {
    super({});

    assert(_.isObject(channelMembership), 'Channel Membership should be an object');
    assert(!!_.get(channelMembership, 'agentId'), 'Channel Membership Agent Id is required');
    assert(!!_.get(channelMembership, 'channelId'), 'Channel Membership Channel Id is required');
    assert(!!_.get(channelMembership, 'type'), 'Channel Membership Type is required');

    this.channelMembership = channelMembership;
    this.templateName =
      Producer.TEMPLATE_NAME.MEMBER_JOINED_TO_COMMUNITY_FEED_ITEM;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    if (this.channelMembership.type !== CHANNEL_MEMBERSHIP_TYPE.JOINED) {
      console.log(this.name, 'Channel Membership is not JOINED');
      return true;
    }

    try {
      const agent = await Producer.data.agents.getOne({
        id: this.channelMembership.agentId,
      });
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');

      const replacements = await this._getReplacements(agent);
      const channelId = this.channelMembership.channelId;
      await this._queueFeedItem(replacements, channelId);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      console.log(this.name, e.message, e);
      return false;
    }
  }

  /**
   * @param {Models.Agent} agent
   * @return {{agent: {name: string}}}
   * @protected
   */
  async _getReplacements(agent) {
    return {
      agent: {
        name: (await _.get(agent, 'name')) || 'Somebody',
      },
    }
  }

}

module.exports = {
  MemberJoinedToCommunity,
};
