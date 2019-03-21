const data = require('../data');

/**
 * @tested
 * @param {Model} channel
 * @param {string} channel.id
 * @returns {Promise<Model[]>}
 */
async function channelAgents(channel) {
  return await data.channelsData.getAgentsByChannelId(channel.id);
}

/**
 * @tested
 * @param {Model} channel
 * @param {string} channel.id
 * @returns {Promise<Model>}
 */
async function channelCreator(channel) {
  return await data.channelsData.getCreatorByChannelId(channel.id);
}

module.exports = {
  channelAgents,
  channelCreator,
};
