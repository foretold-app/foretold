const data = require('../data');

/**
 * @tested
 * @param {Models.Channel} channel
 * @returns {Promise<Model[]>}
 */
async function channelAgents(channel) {
  return await data.channelsData.getAgentsByChannelId(channel.id);
}

/**
 * @tested
 * @param {Models.Channel} channel
 * @returns {Promise<Model>}
 */
async function channelCreator(channel) {
  return await data.channelsData.getCreatorByChannelId(channel.id);
}

/**
 * @tested
 * @param {object | null} root
 * @param {object} args
 * @param {number} args.offset
 * @param {number} args.limit
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel[]>}
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
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function one(root, args, context, info) {
  const id = args.id;
  const agentId = context.user.agentId;
  const channelIds = await data.agentsChannelsData.getAllChannelIds({ agentId });
  const options = { restrictions: { channelIds } };
  return await data.channelsData.getOne(id, options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {string} args.id
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function update(root, args, context, info) {
  return await data.channelsData.updateOne(args.id, args.input);
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function create(root, args, context, info) {
  return await data.channelsData.createOne(context.user, args.input);
}

module.exports = {
  all,
  one,
  create,
  update,
  channelAgents,
  channelCreator,
};
