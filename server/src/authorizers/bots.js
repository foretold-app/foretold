const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {object} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function botBelongsToCurrentUserRule(root, args, context, info) {
  const botUserId = _.get(root, 'userId');
  const userId = _.get(context, 'user.id');
  const result = !!botUserId && botUserId === userId;
  console.log(`\x1b[33m Rule Bots (botBelongsToCurrentUser) "${result}"\x1b[0m`);
  return result;
}

/** @type {Rule} */
const botBelongsToCurrentUser = rule({
  cache: 'no_cache',
})(botBelongsToCurrentUserRule);

module.exports = {
  botBelongsToCurrentUser,
};
