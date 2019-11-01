const _ = require('lodash');
const data = require('../../data');

const { Pagination } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { structures } = require('../../data/classes');

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
 * @param {Models.ObjectID} args.channelMemberId
 * @param {string[]} args.isArchived
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.Channel[]>}
 */
async function all(root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const channelMemberId =
    _.get(args, 'channelMemberId', null) || _.get(root, 'id', null);
  const isArchived = _.get(args, 'isArchived', null);

  const withinJoinedChannels = _.isEmpty(channelMemberId)
    ? null : structures.withinJoinedChannelsById(channelMemberId);

  const filter = new Filter({
    withinJoinedChannels,
    isArchived,
  });
  const pagination = new Pagination(args);
  const options = new Options({ agentId });

  return data.channels.getAll(filter, pagination, options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.Channel>}
 */
async function one(root, args, context, _info) {
  const id = _.get(args, 'id', null) || _.get(root, 'channelId', null);
  const agentId = _.get(context, 'agent.id', null);

  const params = new Params({ id });
  const query = new Query();
  const options = new Options({ agentId });

  return data.channels.getOne(params, query, options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.Channel>}
 */
async function update(root, args, context, _info) {
  const id = _.get(args, 'id', null);
  const params = new Params({ id });
  const data$ = new Data(args.input);
  return data.channels.updateOne(params, data$);
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.Channel>}
 */
async function create(root, args, context, _info) {
  const creatorId = _.get(context, 'agent.id', null);
  const input = _.get(args, 'input') || {};

  return data.channels.createOne({
    ...input,
    creatorId,
  });
}

module.exports = {
  all,
  one,
  create,
  update,
  channelAgents,
  channelCreator,
};
