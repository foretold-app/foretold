const _ = require('lodash');
const { rule } = require('graphql-shield');

const logger = require('../../lib/log');

const log = logger.module('authorizers/channel-bookmarks');

/**
 * @todo: To fix "_.get(root, 'agentId')".
 * @param {object} root
 * @param {Models.ChannelID} root.channelId
 * @param {object} args
 * @param {Models.ChannelID} args.channelId
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function channelBookmarkBelongsToCurrentAgentRule(root, args, context, _info) {
  const channelBookmarkId = _.get(context, 'channelBookmark.agentId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const result = (!!channelBookmarkId && !!currentAgentId)
    && channelBookmarkId === currentAgentId;

  log.trace(
    '\x1b[33m Rule Channel Memberships '
    + '(channelBookmarkBelongsToCurrentAgentRule) channelBookmarkId = '
    + `"${channelBookmarkId}", currentAgentId = "${currentAgentId}", `
    + `result = "${result}".\x1b[0m`,
  );

  return result;
}

/** @type {Rule} */
const channelBookmarkBelongsToCurrentAgent = rule({
  cache: 'no_cache',
})(channelBookmarkBelongsToCurrentAgentRule);

module.exports = {
  channelBookmarkBelongsToCurrentAgent,
};
