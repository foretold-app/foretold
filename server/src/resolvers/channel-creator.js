const data = require('../data');

async function channelCreator(channel) {
  return await data.channelsData.getCreatorByChannelId(channel.id);
}

module.exports = {
  channelCreator,
};
