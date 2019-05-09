const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function channelIsPublicRule(root, args, context, info) {
  const result = !!_.get(context, 'channel.isPublic');
  console.log(`\x1b[33m Rule Channels (channelIsPublic) ` +
    `channelId "${_.get(context, 'channel.id')}", ` +
    `channelIsPublic "${JSON.stringify(result)}"` +
    `\x1b[0m`);
  return result;
}

/** @type {Rule} */
const channelIsPublic = rule({
  cache: 'no_cache',
})(channelIsPublicRule);

module.exports = {
  channelIsPublic,
};
