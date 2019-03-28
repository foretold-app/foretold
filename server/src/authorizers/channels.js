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
 * @param {object} parent
 * @param {object} args
 * @param {{ channel: Models.Channel, agentChannel: Models.AgentChannel }} ctx
 * @param {object} info
 */
async function isChannelAllowedRule(parent, args, ctx, info) {
  const channel = _.get(ctx, 'channel');
  const agentChannel = _.get(ctx, 'agentChannel');
  console.log(`\x1b[33m Rule (isChannelAllowed) ` +
    `channelId ${_.get(channel, 'id')}, ` +
    `agentChannelId ${_.get(agentChannel, 'agentId')} \x1b[0m`);
  return channels.authorize(channel, agentChannel);
}

const isChannelAllowed = rule()(isChannelAllowedRule);

const channels = {
  authorize,
  isChannelAllowed,
  isChannelAllowedRule,
};

module.exports = channels;
