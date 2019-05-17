const _ = require('lodash');
const data = require('../data');
const filters = require('../data/filters');
const paginations = require('../data/paginations');

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.creatorId
 * @param {string} args.seriesId
 * @param {string} args.channelId
 * @param {string} args.measuredByAgentId
 * @param {string} args.resultOrLatestMeasurementForAgentId
 * @param {string[]} args.states
 * @param {string[]} args.isArchived
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Measurable[]>}
 */
async function all(root, args, context, info) {
  const filter = new filters.MeasurableFilter({ options: args });
  const pagination = new paginations.Pagination({ options: args });
  const options = {
    isAdmin: _.get(context, 'agent.isAdmin'),
    agentId: _.get(context, 'agent.id'),
    measuredByAgentId: _.get(args, 'measuredByAgentId'),
  };
  // tricky
  context.resultOrLatestMeasurementForAgentId = args.resultOrLatestMeasurementForAgentId;
  const result = await data.measurables.getAll(filter, pagination, options);
  return result.data;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function one(root, args, context, info) {
  const agentId = _.get(context, 'agent.id');
  return await data.measurables.getOne(args.id, { agentId });
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
  const creator = context.creator;
  const agentId = _.get(context, 'agent.id');
  const datas = {
    ...args.input,
    creatorId: agentId,
  };
  return await data.measurables.createOne(datas, creator);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function archive(root, args, context, info) {
  const { id } = args;
  return await data.measurables.archive(id);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function unarchive(root, args, context, info) {
  const { id } = args;
  return await data.measurables.unArchive(id);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.id
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function update(root, args, context, info) {
  const id = args.id;
  const datas = args.input;
  const creator = context.creator;
  return await data.measurables.updateOne(id, datas, creator);
}


module.exports = {
  one,
  all,
  update,
  unarchive,
  archive,
  create,
};
