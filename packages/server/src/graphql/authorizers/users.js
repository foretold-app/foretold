const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');

const log = logger.module('authorizers/users');

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
    const contextAgentIdIn = _.get(context, 'agent.id', null);

    const agentId = _.toString(agentIdIn);
    const contextAgentId = _.toString(contextAgentIdIn);

    const result = (!!agentId && !!contextAgentId)
      && (agentId === contextAgentId);

    log.trace('\x1b[33m Rule Users (userIsOwnedByCurrentAgentRule) '
      + `agentId "${agentId}", contextAgentId "${contextAgentId}", `
      + `result = "${result}".\x1b[0m`);

    return result;
  };
}

/**
 * @param {function} predicateAgentId
 * @return {Rule}
 */
const userIsOwnedByCurrentAgent = (predicateAgentId) =>
  /** @type {Rule} */
  rule({
    cache: 'no_cache',
  })(userIsOwnedByCurrentAgentRule$(predicateAgentId));

module.exports = {
  userIsOwnedByCurrentAgent,
};
