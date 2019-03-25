const _ = require('lodash');

const data = require('../data');

async function agentsChannels(root, args, context, info) {
  const channelId = args.channelId || root.channelId || context.channelId;
  const agentId = context.user.agentId;
  context.agentChannel = channelId && agentId
    ? await data.agentsChannelsData.getOne({ agentId, channelId })
    : null;
}

module.exports = {
  agentsChannels,
};
