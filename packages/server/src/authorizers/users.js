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
  const userIdIn = _.get(root, 'id');
  const contextUserIdIn = _.get(context, 'user.id');

  const userId = _.toString(userIdIn);
  const contextUserId = _.toString(contextUserIdIn);

  const result = userId === contextUserId;

  console.log(`\x1b[33m Rule Users (userIsOwnedByCurrentAgentRule) ` +
    `userId "${userId}", contextUserId "${contextUserId}", ` +
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
