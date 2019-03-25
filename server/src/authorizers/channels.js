const _ = require('lodash');
const { rule } = require('graphql-shield');

async function authorize(channel, agentChannel) {
  if (!channel) {
    return false;
  }
  if (channel.isPublic) {
    return true;
  }
  if (!agentChannel) {
    return false;
  }
  if (!agentChannel) {
    return false;
  }
  return true;
}

const isChannelAllowed = rule()(async (parent, args, ctx, info) => {
  const channel = _.get(ctx, 'channel');
  const agentChannel = _.get(ctx, 'agentChannel');
  console.log(`\x1b[33m Rule (isChannelAllowed) ` +
    `channelId ${_.get(channel, 'id')}, ` +
    `agentChannelId ${_.get(agentChannel, 'agentId')} \x1b[0m`);
  return await authorize(channel, agentChannel);
});

module.exports = {
  isChannelAllowed,
};
