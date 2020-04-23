const _ = require('lodash');

const { MeasurablesData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Data } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { structures } = require('../../data/classes');

const { HOME_CHANNEL_ID } = require('../../../config/well-known');

/**
 *
 * These is a tiny difference in a sense of "filter" and "restrictions"
 * (which are made using "options" on the data layer). "Filter" is about
 * what one wants to get. "Restrictions" is about what one can get.
 *
 * @todo: update input of getAll
 * @param {*} root
 * @param {object} args
 * @param {Defs.AgentID} args.creatorId
 * @param {Defs.SeriesID} args.seriesId
 * @param {Defs.ChannelID} args.channelId
 * @param {Defs.AgentID} args.measuredByAgentId
 * @param {string[]} args.states
 * @param {string[]} args.isArchived
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  /**
   * @todo: search tag: "withinJoinedChannelsByChannelId"
   * @type {Layers.withinJoinedChannels | null}
   */
  const withinJoinedChannels = channelId === HOME_CHANNEL_ID && !!currentAgentId
    ? structures.withinJoinedChannelsByChannelId(currentAgentId) : null;

  const filter = new Filter({
    channelId,
    withinJoinedChannels,
    measurableIds: _.get(args, 'measurableIds', null),
    creatorId: _.get(args, 'creatorId', null),
    seriesId: _.get(args, 'seriesId', null),
    states: _.get(args, 'states', null),
    isArchived: _.get(args, 'isArchived', null),
    measuredByAgentId: _.get(args, 'measuredByAgentId', null),
    labelProperty: _.get(args, 'labelProperty', null),
    labelSubject: _.get(args, 'labelSubject', null),
  });
  const pagination = new Pagination(args);
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin', null),
    agentId: currentAgentId,
    attributes: true,
    raw: true,
  });

  return new MeasurablesData().getConnection(filter, pagination, options);
}

/**
 * @todo: Use predicates!
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, _info) {
  const measurableId = _.get(args, 'id', null)
    || _.get(root, 'measurableId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const params = new Params({ id: measurableId });
  const query = new Query();
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin', null),
    agentId: currentAgentId,
    raw: true,
  });

  return new MeasurablesData().getOne(params, query, options);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const input = _.get(args, 'input', null);
  const data = new Data({
    ...input,
    creatorId: agentId,
  });
  return new MeasurablesData().createOne(data);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Defs.MeasurableID} args.id
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function archive(root, args, _context, _info) {
  const measurableId = _.get(args, 'id', null);
  return new MeasurablesData().archive(measurableId);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Defs.MeasurableID} args.id
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function unarchive(root, args, _context, _info) {
  const measurableId = _.get(args, 'id', null);
  return new MeasurablesData().unArchive(measurableId);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Defs.MeasurableID} args.id
 * @param {object} args.input
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function update(root, args, _context, _info) {
  const measurableId = _.get(args, 'id', null);
  const input = _.get(args, 'input') || {};
  const params = new Params({ id: measurableId });
  const data = new Data(input);
  return new MeasurablesData().updateOne(params, data);
}

/**
 * @param {*} root
 * @param {Defs.MeasurableID} root.id
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function openedCount(root, _args, _context, _info) {
  const channelId = _.get(root, 'id', null);
  return new MeasurablesData().getOpenedCount(channelId);
}

/**
 * @param {*} _root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function count(_root, _args, _context, _info) {
  return new MeasurablesData().getCount();
}

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function measurableCount(root, _args, _context, _info) {
  const seriesId = _.get(root, 'id', null);
  return new MeasurablesData().getCount(new Params({ seriesId }));
}

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @returns {Promise<*|Array<Model>>}
 */
async function resolutionEndpointResponse(root, _args, _context, _info) {
  return new MeasurablesData().resolutionEndpointResponse(root);
}

module.exports = {
  one,
  all,
  update,
  unarchive,
  archive,
  create,
  openedCount,
  count,
  measurableCount,
  resolutionEndpointResponse,
};
