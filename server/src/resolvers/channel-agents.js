const data = require('../data');

async function channelAgents(channel) {
  return await data.channelsData.getAgentsByChannelId(channel.id);
}

module.exports = {
  channelAgents,
};
