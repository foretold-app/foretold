const _ = require('lodash');
const { rule } = require('graphql-shield');

async function authorize(channel, agentChannel) {
  if (!channel) {
    return Promise.reject(new Error('Channel ID is required'));
  }
  if (!agentChannel) {
    return Promise.reject(new Error('Agent ID is required'));
  }
  if (channel.isPublic) {
    return true;
  }
  if (!agentChannel) {
    return Promise.reject(new Error('Access denied'));
  }
  return true;
}

const isChannelAllowed = rule()(async (parent, args, ctx, info) => {
  const channel = _.get(ctx, 'channel');
  const agentChannel = _.get(ctx, 'agentChannel');
  return await authorize(channel, agentChannel);
});

module.exports = {
  isChannelAllowed,
};
