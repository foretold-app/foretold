const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isAuthenticatedRule(root, args, context, info) {
  const userId = _.get(context, 'user.id');
  console.log(`\x1b[33m Rule (isAuthenticatedRule) userId ${userId} \x1b[0m`);
  return !!userId;
}

/** @type {Rule} */
const isAuthenticated = rule()(isAuthenticatedRule);

module.exports = {
  isAuthenticated,
};
