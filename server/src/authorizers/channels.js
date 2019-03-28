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
  const channelIsPublic = _.get(context, 'channel.isPublic', false);
  console.log(`\x1b[33m Rule (isChannelPublic) ` +
    `channelId "${_.get(channel, 'id')}" \x1b[0m`);
  return !!channelIsPublic;
}

/** @type {Rule} */
const isChannelPublic = rule()(isChannelPublicRule);

module.exports = {
  isChannelPublic,
};
