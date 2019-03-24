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

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function channels(root, args, context, info) {
  const agentId = context.user.agentId;
  const options = {
    restrictions: { agentId },
  };
  return await data.channelsData.getAll(options);
}

module.exports = {
  channelAgents,
  channelCreator,
  channels,
};
