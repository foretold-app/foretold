const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {Models.Channel} channel
 * @param {Models.AgentChannel} agentChannel
 * @return {boolean}
 */
function authorize(channel, agentChannel) {
  if (!channel) return false;
  if (channel.isPublic) return true;
  if (!agentChannel) return false;
  return true;
}

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isChannelAllowedRule(root, args, context, info) {
  const channel = _.get(context, 'channel');
  const agentChannel = _.get(context, 'agentChannel');
  console.log(`\x1b[33m Rule (isChannelAllowed) ` +
    `channelId ${_.get(channel, 'id')}, ` +
    `agentChannelId ${_.get(agentChannel, 'agentId')} \x1b[0m`);
  return channels.authorize(channel, agentChannel);
}

/** @type {Rule} */
const isChannelAllowed = rule()(isChannelAllowedRule);

const channels = {
  authorize,
  isChannelAllowed,
  isChannelAllowedRule,
};

module.exports = channels;
