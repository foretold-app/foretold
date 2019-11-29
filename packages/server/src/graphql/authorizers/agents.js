const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');

const log = logger.module('authorizers/agents');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function currentAgentIsAuthenticatedRule(_root, args, context, _info) {
  const agentId = _.get(context, 'agent.id', null);
  const result = !!agentId;

  log.trace(
    '\x1b[33m Rule Agents (currentAgentIsAuthenticatedRule) '
    + `agentId "${agentId}", result = "${result}".\x1b[0m`,
  );

  return result;
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function currentAgentIsApplicationAdminRule(_root, args, context, _info) {
  const isAdmin = _.get(context, 'agent.isAdmin', false);
  const result = !!isAdmin;

  log.trace(
    '\x1b[33m Rule Agents (currentAgentIsAdminRule) '
    + `result = "${result}".\x1b[0m`,
  );

  return result;
}

/** @type {Rule} */
const currentAgentIsAuthenticated = rule({
  cache: 'no_cache',
})(currentAgentIsAuthenticatedRule);

/** @type {Rule} */
const currentAgentIsApplicationAdmin = rule({
  cache: 'no_cache',
})(currentAgentIsApplicationAdminRule);

module.exports = {
  currentAgentIsAuthenticated,
  currentAgentIsApplicationAdmin,
  currentAgentIsAuthenticatedRule,
};
