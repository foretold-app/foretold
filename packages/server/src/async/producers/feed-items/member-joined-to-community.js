const assert = require('assert');
const _ = require('lodash');

const { getMeasurableLinkWithToken } = require('../../../lib/urls');

const { ProducerFeedItems } = require('./producer-feed-items');

class MemberJoinedToCommunity extends ProducerFeedItems {

  constructor(channelMembership = {}) {
    super({});
    assert(_.isObject(channelMembership), 'Channel Membership should be an object');
    this.channelMembership = channelMembership;
  }

  /**
   * @param {object} channel
   * @param {object} measurable
   * @return {{measurable: {name: *, link: *}}}
   * @protected
   */
  static _getReplacements(channel, measurable) {
    return {
      measurable: {
        name: _.get(measurable, 'name'),
        link: getMeasurableLinkWithToken(channel, measurable),
      },
    }
  }

}

module.exports = {
  MemberJoinedToCommunity,
};
