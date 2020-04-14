const _ = require('lodash');

/**
 * @todo: When I had been working on this file
 * @todo: I did not know what the "predicate" is.
 */

/**
 * @todo: But in programing language, “predicate” just means
 * @todo: a function that return true or false.
 */

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {string | null}
 */
const agentIdFromRootId = (root, _args, _context, _info) => {
  return _.get(root, 'id', null);
};

/**
 * @param {*} _root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {string | null}
 */
const agentIdFromContext = (_root, _args, context, _info) => {
  return _.get(context, 'userAsObject.agentId', null);
};

/**
 * @param {*} root
 * @param {object} _args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {string | null}
 */
const agentIdFromRootAgentId = (root, _args, _context, _info) => {
  return _.get(root, 'agentId', null);
};

module.exports = {
  agentIdFromRootId,
  agentIdFromRootAgentId,
  agentIdFromContext,
};
