const _ = require('lodash');

const data = require('../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function agentsChannels(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId');
  const agentId = _.get(context, 'user.agentId') ;
  console.log('\x1b[36m ---> \x1b[0m Middleware (agentsChannels)', { agentId, channelId });
  context.agentChannel = channelId && agentId
    ? await data.agentsChannelsData.getOne({ agentId, channelId })
    : null;
}

module.exports = {
  agentsChannels,
};
