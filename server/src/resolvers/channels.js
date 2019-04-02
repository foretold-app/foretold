const _ = require('lodash');
const data = require('../data');

/**
 * @param {Models.Channel} channel
 * @returns {Promise<Model[]>}
 */
async function channelAgents(channel) {
  return await data.channels.getAgentsByChannelId(channel.id);
}

/**
 * @param {Models.Channel} channel
 * @returns {Promise<Model>}
 */
async function channelCreator(channel) {
  return await data.channels.getCreatorByChannelId(channel.id);
}

/**
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
  const options = { offset, limit, agentId };
  return await data.channels.getAll(options);
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
  const id = _.get(args, 'id') || _.get(root, 'channelId');
  const agentId = _.get(context, 'user.agentId');
  const options = { agentId };
  return await data.channels.getOne(id, options);
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
  return await data.channels.updateOne(args.id, args.input);
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function create(root, args, context, info) {
  return await data.channels.createOne(context.user, args.input);
}

module.exports = {
  all,
  one,
  create,
  update,
  channelAgents,
  channelCreator,
};
