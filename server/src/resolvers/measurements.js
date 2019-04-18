const _ = require('lodash');
const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  const agentId = _.get(context, 'agent.id');
  const all = await data.measurements.getAll({ agentId });
  return {
    total: 10,
    pageInfo: {
      hasNextPage: true,
      hasPreviousPage: true,
      startCursor: '1',
      endCursor: '2',
    },
    edges: [
      { node: all[0], cursor: '3'}
    ],
  }
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
