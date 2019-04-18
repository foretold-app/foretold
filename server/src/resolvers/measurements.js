const _ = require('lodash');
const data = require('../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function all(root, args, context, info) {
  /** @type {string} */
  const agentId = _.get(context, 'agent.id');
  const filter = {
    measurableId: _.get(args, 'measurableId'),
    agentId: _.get(args, 'agentId'),
    after: _.get(args, 'after'),
    before: _.get(args, 'before'),
  };
  const pagination = {
    offset: _.get(args, 'last'),
    limit: _.get(args, 'first'),
  };
  const options = { agentId };
  return await data.measurements.getAll(filter, pagination, options);
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
