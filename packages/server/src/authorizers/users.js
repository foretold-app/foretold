const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {function} predicateAgentId
 * @return {function(*=, *=, *=, *=): boolean}
 */
function userIsOwnedByCurrentAgentRule$(predicateAgentId) {
  /**
   * @param {*} root
   * @param {object} args
   * @param {Schema.Context} context
   * @param {object} info
   * @return {boolean}
   */
  return function userIsOwnedByCurrentAgentRule(root, args, context, info) {
    const agentIdIn = predicateAgentId(root, args, context, info);
    const contextAgentIdIn = _.get(context, 'agent.id');

    const agentId = _.toString(agentIdIn);
    const contextAgentId = _.toString(contextAgentIdIn);

    const result =
      (!!agentId && !!contextAgentId)
      && (agentId === contextAgentId);

    console.log(`\x1b[33m Rule Users (userIsOwnedByCurrentAgentRule) ` +
      `agentId "${agentId}", contextAgentId "${contextAgentId}", ` +
      `result = "${result}".\x1b[0m`);

    return result;
  };
}

/**
 * @param {function} predicateAgentId
 * @return {Rule}
 */
const userIsOwnedByCurrentAgent = (predicateAgentId) => {
  /** @type {Rule} */
  return rule({
    cache: 'no_cache',
  })(userIsOwnedByCurrentAgentRule$(predicateAgentId));
};

module.exports = {
  userIsOwnedByCurrentAgent,
};
