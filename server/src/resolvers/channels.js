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
  const offset = args.offset;
  const limit = args.limit;
  const channelIds = await data.agentsChannelsData.getAllChannelIds({ agentId });
  const options = { offset, limit, restrictions: { channelIds } };
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

/**
 * @param root
 * @param args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function update(root, args, context, info) {
  return await data.channelsData.updateOne(args.id, args.input);
}

/**
 * @param root
 * @param {{ input: Schema.ChannelsInput }} args
 * @param context
 * @param info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, info) {
  return await data.channelsData.createOne(options.user, args.input);
}

module.exports = {
  all,
  one,
  create,
  update,
  channelAgents,
  channelCreator,
};
