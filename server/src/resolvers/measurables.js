const _ = require('lodash');
const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.creatorId
 * @param {string} args.seriesId
 * @param {string} args.channelId
 * @param {string} args.measuredByAgentId
 * @param {string[]} args.states
 * @param {string[]} args.isArchived
 * @param {boolean} args.isAggregated
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Measurable[]>}
 */
async function all(root, args, context, info) {
  const filter = {
    creatorId: _.get(args, 'creatorId'),
    seriesId: _.get(args, 'seriesId'),
    channelId: _.get(args, 'channelId'),
    states: _.get(args, 'states'),
    isArchived: _.get(args, 'isArchived'),
    isAggregated: _.get(args, 'isAggregated'),
  };
  const pagination = {
    last: _.get(args, 'last'),
    first: _.get(args, 'first'),
    after: _.get(args, 'after'),
    before: _.get(args, 'before'),
  };
  const options = {
    agentId: _.get(context, 'agent.id'),
    measuredByAgentId: _.get(args, 'measuredByAgentId'),
  };
  const result = await data.measurables.getAll(filter, pagination, options);
  context.total = result.total;
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
 * @param root
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

/**
 * @param {*} root
 * @param {object} args
 * @param {string} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function aggregate(root, args, context, info) {
  const id = args.id;
  return await data.measurables.markAggregated(id);
}


module.exports = {
  one,
  all,
  create,
  update,
  archive,
  unarchive,
  aggregate,
};
