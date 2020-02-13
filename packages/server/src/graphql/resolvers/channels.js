const _ = require('lodash');

const { ChannelsData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { structures } = require('../../data/classes');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {number} args.offset
 * @param {number} args.limit
 * @param {Models.AgentID} args.channelMemberId
 * @param {string[]} args.isArchived
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.Channel[]>}
 */
async function all(root, args, context, _info) {
  const currentAgentId = _.get(context, 'agent.id', null);
  const channelMemberId = _.get(args, 'channelMemberId', null)
    || _.get(root, 'id', null);
  const isArchived = _.get(args, 'isArchived', null);

  const withinJoinedChannels = _.isEmpty(channelMemberId)
    ? null : structures.withinJoinedChannelsById(channelMemberId);

  const filter = new Filter({ withinJoinedChannels, isArchived });
  const pagination = new Pagination(args);
  const options = new Options({
    raw: true,
    attributes: {
      bookmarksCount: true,
      isBookmarked: { agentId: currentAgentId },
    },
    agentId: currentAgentId,
  });

  const response = await new ChannelsData().getConnection(
    filter, pagination, options,
  );

  return response.getData();
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ChannelID} args.id
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<Models.Channel>}
 */
async function one(root, args, context, _info) {
  const channelId = _.get(args, 'id', null) || _.get(root, 'channelId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const params = new Params({ id: channelId });
  const query = new Query();
  const options = new Options({
    raw: true,
    attributes: {
      bookmarksCount: true,
      isBookmarked: { agentId: currentAgentId },
    },
    agentId: currentAgentId,
  });

  return new ChannelsData().getOne(params, query, options);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ChannelID} args.id
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<Models.Channel>}
 */
async function update(root, args, _context, _info) {
  const channelId = _.get(args, 'id', null);

  const params = new Params({ id: channelId });
  const data$ = new Data(args.input);

  return new ChannelsData().updateOne(params, data$);
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

  return new ChannelsData().createOne(new Data({
    ...input,
    creatorId,
  }));
}

module.exports = {
  all,
  one,
  create,
  update,
};
