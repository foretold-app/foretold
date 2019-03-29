const _ = require('lodash');

const data = require('../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function agentsChannels(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId')
    || _.get(context, 'channel.id');
  const agentId = _.get(context, 'user.agentId');
  const id = { agentId, channelId };
  console.log('\x1b[36m ---> \x1b[0m Middleware (agentsChannels)', id);
  if (channelId && agentId) {
    const agentChannel = await data.agentsChannelsData.getOne(id);
    context.agentChannel = agentChannel;
    context.agentsChannelsRole = _.get(agentChannel, 'role', []);
  } else {
    context.agentChannel = null;
    context.agentsChannelsRole = null;
  }
}

module.exports = {
  agentsChannels,
};
