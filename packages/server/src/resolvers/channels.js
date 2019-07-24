const _ = require('lodash');
const data = require('../data');

const { Pagination } = require('../data/classes/pagination');

/**
 * @param {Models.Channel} channel
 * @returns {Promise<Model[]>}
 */
async function channelAgents(channel) {
  return data.channels.getAgentsByChannelId(channel.id);
}

/**
 * @param {Models.Channel} channel
 * @returns {Promise<Model>}
 */
async function channelCreator(channel) {
  return data.channels.getCreatorByChannelId(channel.id);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {number} args.offset
 * @param {number} args.limit
 * @param {number} args.channelMemberId
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel[]>}
 */
async function all(root, args, context, info) {
  const agentId = _.get(context, 'agent.id');
  const channelMemberId = _.get(args, 'channelMemberId');
  const filter = { channelMemberId };
  const pagination = new Pagination(args);
  const options = { agentId };
  return data.channels.getAll(filter, pagination, options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function one(root, args, context, info) {
  const id = _.get(args, 'id') || _.get(root, 'channelId');
  const agentId = _.get(context, 'agent.id');
  const params = { id };
  const query = {};
  const options = { agentId };
  return data.channels.getOne(params, query, options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function update(root, args, context, info) {
  const params = { id: args.id };
  return data.channels.updateOne(params, args.input);
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function create(root, args, context, info) {
  return data.channels.createOne(context.agent, args.input);
}

module.exports = {
  all,
  one,
  create,
  update,
  channelAgents,
  channelCreator,
};
