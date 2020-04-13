const _ = require('lodash');
const { rule } = require('graphql-shield');

const logger = require('../../lib/log');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../enums');

const log = logger.module('authorizers/measurement');

/**
 * @param {object} _root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function measurementIsCompetitiveOrCommentOnlyRule(
  _root, _args, context, _info,
) {
  const competitorType = _.get(context, 'measurement.competitorType', null);

  const result = !_.isEmpty(competitorType)
    && (MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE === competitorType
      || MEASUREMENT_COMPETITOR_TYPE.COMMENT === competitorType);

  log.trace(
    '\x1b[33m Rule Measurements '
    + `(measurementIsCompetitiveOrCommentOnlyRule) "${result}".\x1b[0m`,
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
function measurementIsOwnedByCurrentAgentRule(
  _root, _args, context, _info,
) {
  const measurementAgentId = _.get(context, 'measurement.agentId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const result = (!!measurementAgentId && !!currentAgentId)
    && measurementAgentId === currentAgentId;

  log.trace(
    '\x1b[33m Rule Measurements (measurementIsOwnedByCurrentAgentRule), '
    + `result = "${result}".\x1b[0m`,
  );

  return result;
}

/** @type {Rule} */
const measurementIsCompetitiveOrCommentOnly = rule({
  cache: 'no_cache',
})(measurementIsCompetitiveOrCommentOnlyRule);

/** @type {Rule} */
const measurementIsOwnedByCurrentAgent = rule({
  cache: 'no_cache',
})(measurementIsOwnedByCurrentAgentRule);

module.exports = {
  measurementIsCompetitiveOrCommentOnly,
  measurementIsOwnedByCurrentAgent,
};
