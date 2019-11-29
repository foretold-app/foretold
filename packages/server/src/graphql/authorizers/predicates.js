const _ = require('lodash');

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {string | undefined}
 */
const agentIdFromRootId = (root, _args, _context, _info) => {
  return _.get(root, 'id', null);
};

/**
 * @param {*} _root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {string | undefined}
 */
const agentIdFromContext = (_root, _args, context, _info) => {
  return _.get(context, 'userAsObject.agentId', null);
};

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {string | undefined}
 */
const agentIdFromRootAgentId = (root, _args, _context, _info) => {
  return _.get(root, 'agentId', null);
};

module.exports = {
  agentIdFromRootId,
  agentIdFromRootAgentId,
  agentIdFromContext,
};
