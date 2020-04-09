const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');

const log = logger.module('authorizers/channels');

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function channelIsPublicRule(root, args, context, _info) {
  const result = !!_.get(context, 'channel.isPublic', null);

  log.trace(
    '\x1b[33m Rule Channels (channelIsPublic) '
    + `channelId "${_.get(context, 'channel.id', null)}", `
    + `channelIsPublic "${JSON.stringify(result)}".`
    + '\x1b[0m',
  );

  return result;
}

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function channelRequiresVerificationRule(root, args, context, _info) {
  const result = !!_.get(context, 'channel.requireVerification', null);

  log.trace(
    '\x1b[33m Rule Channels (channelRequiresVerificationRule) '
    + `channelId "${_.get(context, 'channel.id', null)}", `
    + `requireVerification "${JSON.stringify(result)}".`
    + '\x1b[0m',
  );

  return result;
}

/** @type {Rule} */
const channelIsPublic = rule({
  cache: 'no_cache',
})(channelIsPublicRule);

/** @type {Rule} */
const channelRequiresVerification = rule({
  cache: 'no_cache',
})(channelRequiresVerificationRule);

module.exports = {
  channelIsPublic,
  channelRequiresVerification,
};
