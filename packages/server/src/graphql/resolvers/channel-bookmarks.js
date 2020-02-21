const _ = require('lodash');

const { ChannelBookmarksData } = require('../../data');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Definitions.ChannelID} args.channelId
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<boolean>}
 */
async function toggle(_root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null);
  const currentAgentId = _.get(context, 'agent.id', null);
  return new ChannelBookmarksData().toggle(channelId, currentAgentId);
}

module.exports = {
  toggle,
};
