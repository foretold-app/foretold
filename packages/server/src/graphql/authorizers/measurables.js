const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');

const log = logger.module('authorizers/measurables');

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {boolean}
 */
function measurableIsOwnedByCurrentAgentRule(root, args, context, info) {
  const creatorId = _.get(context, 'measurable.creatorId', null);
  const agentId = _.get(context, 'agent.id', null);

  const result = (!!creatorId && !!agentId)
    && (creatorId === agentId);

  log.trace(
    '\x1b[33m Rule Measurables (measurableIsOwnedByCurrentAgent) '
    + `"${result}".\x1b[0m`,
  );

  const compoundId = { creatorId, agentId };
  log.trace('\x1b[36m ---> \x1b[0m Rule Measurable '
    + '(measurableIsOwnedByCurrentAgent parameters)', compoundId);

  return result;
}

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {boolean}
 */
function measurableIsArchivedRule(root, args, context, info) {
  const result = !!_.get(context, 'measurable.isArchived', null);

  log.trace(
    '\x1b[33m Rule Measurables '
    + `(measurableIsArchivedRule) "${result}".\x1b[0m`,
  );

  return result;
}

/** @type {Rule} */
const measurableIsOwnedByCurrentAgent = rule({
  cache: 'no_cache',
})(measurableIsOwnedByCurrentAgentRule);

/** @type {Rule} */
const measurableIsArchived = rule({
  cache: 'no_cache',
})(measurableIsArchivedRule);

module.exports = {
  measurableIsOwnedByCurrentAgent,
  measurableIsArchived,
};
