const _ = require('lodash');
const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {string} args.measurableId
 * @param {string} args.agentId
 * @param {string[]} args.competitorType
 * @param {string} args.createdAfter
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Measurement[]>}
 */
async function all(root, args, context, info) {
  /** @type {string} */
  const agentId = _.get(context, 'agent.id');
  const filter = {
    measurableId: _.get(args, 'measurableId'),
    agentId: _.get(args, 'agentId'),
    competitorType: _.get(args, 'competitorType'),
    createdAfter: _.get(args, 'createdAfter'),
  };
  const pagination = {
    after: _.get(args, 'after'),
    before: _.get(args, 'before'),
    last: _.get(args, 'last'),
    first: _.get(args, 'first'),
  };
  const options = { agentId };
  const result = await data.measurements.getAll(filter, pagination, options);
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
  return await data.measurements.getOne(args.id, { agentId });
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
  const datas = { ...args.input, agentId };
  return await data.measurements.createOne(datas, creator);
}

module.exports = {
  one,
  all,
  create,
};
