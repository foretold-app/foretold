const _ = require('lodash');
const {rule} = require('graphql-shield');
const {getGraphQLRateLimiter} = require('graphql-rate-limit');

const logger = require('../../lib/log');

const log = logger.module('authorizers/rate-limit');

const rateLimiter = getGraphQLRateLimiter({
  identifyContext: (context) => {
    return _.get(context, 'agent.id');
  }
});

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function rateLimitRule(root, args, context, info) {
  const errorMessage = await rateLimiter(
    {parent: root, args, context, info},
    {max: 2, window: '10s'}
  );

  log.trace('\x1b[33m Rule Rate Limit (rateLimitRule) '
    + `result = "${errorMessage}".\x1b[0m`);

  return !!errorMessage;
}

/** @type {Rule} */
const rateLimit = rule({
  cache: 'no_cache',
})(rateLimitRule);

module.exports = {
  rateLimit,
};
