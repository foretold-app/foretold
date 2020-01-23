const _ = require('lodash');
const { rule } = require('graphql-shield');
const logger = require('../../lib/log');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums');

const log = logger.module('authorizers/measurables');

/**
 * @todo: To fix ".get(context, 'measurable.creatorId', null)".
 * @todo: Either from a context or from a root. And everywhere should be
 * @todo: the same.
 * @param {object} _root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function measurableIsOwnedByCurrentAgentRule(_root, _args, context, _info) {
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
 * @todo: To think about "_.get(context, 'measurable.isArchived', null)".
 * @param {object} _root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function measurableIsArchivedRule(_root, _args, context, _info) {
  const result = !!_.get(context, 'measurable.isArchived', null);

  log.trace(
    '\x1b[33m Rule Measurables '
    + `(measurableIsArchivedRule) "${result}".\x1b[0m`,
  );

  return result;
}

/**
 * @param {object} _root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function measurableIsCompetitiveOrCommentOnlyRule(
  _root, _args, context, _info,
) {
  const competitorType = _.get(context, 'measurable.competitorType', null);

  const result = !!competitorType
    && (MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE === competitorType
      || MEASUREMENT_COMPETITOR_TYPE.COMMENT === competitorType);

  log.trace(
    '\x1b[33m Rule Measurables '
    + `(measurableIsCompetitiveOrCommentOnlyRule) "${result}".\x1b[0m`,
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

/** @type {Rule} */
const measurableIsCompetitiveOrCommentOnly = rule({
  cache: 'no_cache',
})(measurableIsCompetitiveOrCommentOnlyRule);

module.exports = {
  measurableIsOwnedByCurrentAgent,
  measurableIsArchived,
  measurableIsCompetitiveOrCommentOnly,
};
