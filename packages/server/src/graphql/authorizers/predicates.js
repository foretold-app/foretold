const _ = require('lodash');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {string | undefined}
 */
const agentIdFromRootId = (root, args, context, info) => {
  return _.get(root, 'id');
};

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {string | undefined}
 */
const agentIdFromContext = (root, args, context, info) => {
  return _.get(context, 'userAsObject.agentId');
};

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {string | undefined}
 */
const agentIdFromRootAgentId = (root, args, context, info) => {
  return _.get(root, 'agentId');
};

module.exports = {
  agentIdFromRootId,
  agentIdFromRootAgentId,
  agentIdFromContext,
};
