const _ = require('lodash');

const data = require('../../data');

const { Pagination } = require('../../data/classes/pagination');
const { Params } = require('../../data/classes/params');
const { Filter } = require('../../data/classes/filter');
const { Options } = require('../../data/classes/options');
const { Query } = require('../../data/classes/query');

const structures = require('../../structures');

/**
 *
 * These is a tiny difference in a sense of "filter" and "restrictions"
 * (which are made using "options" on the data layer). "Filter" is about
 * what one wants to get. "Restrictions" is about what one can get.
 *
 * @todo: update input of getAll
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.creatorId
 * @param {Models.ObjectID} args.seriesId
 * @param {Models.ObjectID} args.channelId
 * @param {Models.ObjectID} args.measuredByAgentId
 * @param {string} args.resultOrLatestMeasurementForAgentId
 * @param {string[]} args.states
 * @param {string[]} args.isArchived
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function all(root, args, context, info) {
  const channelId = _.get(args, 'channelId');
  const currentAgentId = _.get(context, 'agent.id');

  const withinJoinedChannels = _.isEmpty(channelId)
    ? structures.withinJoinedChannelsByChannelId(currentAgentId) : null;

  const filter = new Filter({
    channelId,
    withinJoinedChannels,
    creatorId: _.get(args, 'creatorId'),
    seriesId: _.get(args, 'seriesId'),
    states: _.get(args, 'states'),
    isArchived: _.get(args, 'isArchived'),
  });
  const pagination = new Pagination(args);
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin'),
    agentId: currentAgentId,
    // @todo: move to filter
    measuredByAgentId: _.get(args, 'measuredByAgentId'),
  });

  // @todo: tricky, rework it.
  context.resultOrLatestMeasurementForAgentId = args.resultOrLatestMeasurementForAgentId;
  return data.measurables.getConnection(filter, pagination, options);
}

/**
 * @todo: Use predicates!
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  const id = _.get(args, 'id')
    || _.get(root, 'measurableId');
  const currentAgentId = _.get(context, 'agent.id');

  const params = new Params({ id });
  const query = new Query();
  const options = new Options({
    isAdmin: _.get(context, 'agent.isAdmin'),
    agentId: currentAgentId,
  });

  return data.measurables.getOne(params, query, options);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function create(root, args, context, info) {
  const { creator } = context;
  const agentId = _.get(context, 'agent.id');
  const datas = {
    ...args.input,
    creatorId: agentId,
  };
  return data.measurables.createOne(datas, creator);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function archive(root, args, context, info) {
  const { id } = args;
  return data.measurables.archive(id);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function unarchive(root, args, context, info) {
  const { id } = args;
  return data.measurables.unArchive(id);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function update(root, args, context, info) {
  const { id } = args;
  const datas = args.input;
  const { creator } = context;
  return data.measurables.updateOne(id, datas, creator);
}

/**
 * @param {*} root
 * @param {Models.ObjectID} root.id
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function openedCount(root, args, context, info) {
  const channelId = root.id;
  return data.measurables.getOpenedCount(channelId);
}

module.exports = {
  one,
  all,
  update,
  unarchive,
  archive,
  create,
  openedCount,
};
