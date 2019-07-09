const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function userIsOwnedByCurrentAgentRule(root, args, context, info) {
  const agentIdIn =
    _.get(root, 'agentId') ||
    _.get(context, 'userAsObject.agentId')
  ;
  const contextAgentIdIn = _.get(context, 'agent.id');

  const agentId = _.toString(agentIdIn);
  const contextAgentId = _.toString(contextAgentIdIn);

  const result = agentId === contextAgentId;

  console.log(`\x1b[33m Rule Users (userIsOwnedByCurrentAgentRule) ` +
    `agentId "${agentId}", contextAgentId "${contextAgentId}", ` +
    `result = "${result}"\x1b[0m`);

  return result;
}

/** @type {Rule} */
const userIsOwnedByCurrentAgent = rule({
  cache: 'no_cache',
})(userIsOwnedByCurrentAgentRule);


module.exports = {
  userIsOwnedByCurrentAgent,
};
