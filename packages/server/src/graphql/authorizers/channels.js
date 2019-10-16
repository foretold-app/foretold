const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');

const log = logger.module('authorizers/channels');

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {boolean}
 */
function channelIsPublicRule(root, args, context, info) {
  const result = !!_.get(context, 'channel.isPublic');

  log.trace(
    '\x1b[33m Rule Channels (channelIsPublic) '
    + `channelId "${_.get(context, 'channel.id')}", `
    + `channelIsPublic "${JSON.stringify(result)}".`
    + '\x1b[0m',
  );

  return result;
}

/** @type {Rule} */
const channelIsPublic = rule({
  cache: 'no_cache',
})(channelIsPublicRule);

module.exports = {
  channelIsPublic,
};
