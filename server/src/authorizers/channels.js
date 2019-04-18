const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isChannelPublicRule(root, args, context, info) {
  const isChannelPublic = !!_.get(context, 'channel.isPublic');
  console.log(`\x1b[33m Rule Channels (isChannelPublic) ` +
    `channelId "${_.get(context, 'channel.id')}", ` +
    `isChannelPublic "${JSON.stringify(isChannelPublic)}"` +
    ` \x1b[0m`);
  return isChannelPublic;
}

/** @type {Rule} */
const isChannelPublic = rule({ cache: 'no_cache' })(isChannelPublicRule);

module.exports = {
  isChannelPublic,
  isChannelPublicRule,
};
