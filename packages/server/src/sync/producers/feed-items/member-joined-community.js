const assert = require('assert');
const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');
const { CHANNEL_MEMBERSHIP_TYPE } = require('../../../enums');
const { Producer } = require('../producer');

class MemberJoinedCommunity extends ProducerFeedItems {
  /**
   * @param {Defs.ChannelMembership} input
   */
  constructor(input) {
    super(input);

    assert(
      !!_.get(input, 'methodCreatedBy'),
      'Channel Membership "Method Created By" is required.',
    );

    this.templateName = Producer.TEMPLATE_NAME
      .MEMBER_JOINED_COMMUNITY_FEED_ITEM;
    this.FeedItem = Producer.FeedItemJoinedMember;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    const { methodCreatedBy } = this.input;
    return methodCreatedBy === CHANNEL_MEMBERSHIP_TYPE.AGENT_JOINED_DIRECTLY;
  }
}

module.exports = {
  MemberJoinedCommunity,
};
