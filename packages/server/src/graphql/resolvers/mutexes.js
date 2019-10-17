const _ = require('lodash');
const { MutexesData } = require('../../data');

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function take(root, args, context, info) {
  const agentId = _.get(context, 'agent.id');
  const name = _.get(args, 'input.name');
  return new MutexesData().take(agentId, name);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*|Array<Model>>}
 */
async function free(root, args, context, info) {
  const agentId = _.get(context, 'agent.id');
  const mutexId = _.get(args, 'id');
  return new MutexesData().free(agentId, mutexId);
}

module.exports = {
  take,
  free,
};
