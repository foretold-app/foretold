const _ = require('lodash');
const { rule } = require('graphql-shield');
const { getGraphQLRateLimiter } = require('graphql-rate-limit');

const logger = require('../../lib/log');

const log = logger.module('authorizers/rate-limit');

const rateLimiter = getGraphQLRateLimiter({
  identifyContext: (context) => {
    return _.get(context, 'ip', null);
  },
});

/**
 * Here we need to make empiric tests.
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function rateLimitRule(root, args, context, info) {
  const errorMessage = await rateLimiter({
    parent: root,
    args,
    context,
    info,
  }, {
    max: 50,
    window: '1s',
  });

  const result = !!!errorMessage;

  log.trace('\x1b[33m Rule Rate Limit (rateLimitRule) '
    + `result = "${result}".\x1b[0m`);

  return result;
}

/** @type {Rule} */
const rateLimit = rule({
  cache: 'no_cache',
})(rateLimitRule);

module.exports = {
  rateLimit,
};
