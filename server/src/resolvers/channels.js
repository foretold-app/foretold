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
async function all(root, args, context, info) {
  const agentId = context.user.agentId;
  const channelIds = await data.agentsChannelsData.getAllChannelIds({ agentId });
  const options = { restrictions: { channelIds } };
  return await data.channelsData.getAll(options);
}

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  const id = args.id;
  const agentId = context.user.agentId;
  const channelIds = await data.agentsChannelsData.getAllChannelIds({ agentId });
  const options = { restrictions: { channelIds } };
  return await data.channelsData.getOne(id, options);
}

module.exports = {
  channelAgents,
  channelCreator,
  all,
  one,
};
