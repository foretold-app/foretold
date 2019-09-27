const _ = require('lodash');
const data = require('../../data');

const { Pagination } = require('../../data/classes/pagination');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');
const { Params } = require('../../data/classes/params');
const { Query } = require('../../data/classes/query');
const { Data } = require('../../data/classes/data');

const structures = require('../../structures');

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
 * @param {object} info
 * @returns {Promise<Models.Channel[]>}
 */
async function all(root, args, context, info) {
  const agentId = _.get(context, 'agent.id');
  const channelMemberId = _.get(args, 'channelMemberId') || _.get(root, 'id');
  const isArchived = _.get(args, 'isArchived');

  const withinJoinedChannels = _.isEmpty(channelMemberId)
    ? null : structures.withinJoinedChannelsById(channelMemberId);

  const filter = new Filter({
    withinJoinedChannels,
    isArchived
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
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function one(root, args, context, info) {
  const id = _.get(args, 'id') || _.get(root, 'channelId');
  const agentId = _.get(context, 'agent.id');

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
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function update(root, args, context, info) {
  const params = new Params({ id: args.id });
  const data$ = new Data(args.input);
  return data.channels.updateOne(params, data$);
}

/**
 * @param {object | null} root
 * @param {{ input: Schema.ChannelsInput }} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Channel>}
 */
async function create(root, args, context, info) {
  const creatorId = _.get(context, 'agent.id');
  const input = {
    ...args.input,
    creatorId
  };
  return data.channels.createOne(input);
}

module.exports = {
  all,
  one,
  create,
  update,
  channelAgents,
  channelCreator,
};
