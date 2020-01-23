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

/** @type {Rule} */
const measurementIsCompetitiveOrCommentOnly = rule({
  cache: 'no_cache',
})(measurementIsCompetitiveOrCommentOnlyRule);

module.exports = {
  measurementIsCompetitiveOrCommentOnly,
};
