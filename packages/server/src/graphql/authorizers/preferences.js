const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');

const log = logger.module('authorizers/preferences');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {boolean}
 */
function preferenceIsOwnedByCurrentAgentRule(root, args, context, info) {
  const agentIdIn = _.get(context, 'preference.agentId');
  const contextAgentIdIn = _.get(context, 'agent.id');

  const agentId = _.toString(agentIdIn);
  const contextAgentId = _.toString(contextAgentIdIn);

  const result = (!!agentId && !!contextAgentId)
    && (agentId === contextAgentId);

  log.trace(
    '\x1b[33m Rule Preferences (preferenceIsOwnedByCurrentAgentRule) '
    + `agentId "${agentId}", contextAgentId "${contextAgentId}", `
    + `result = "${result}".\x1b[0m`,
  );

  return result;
}

/** @type {Rule} */
const preferenceIsOwnedByCurrentAgent = rule({
  cache: 'no_cache',
})(preferenceIsOwnedByCurrentAgentRule);


module.exports = {
  preferenceIsOwnedByCurrentAgent,
};
