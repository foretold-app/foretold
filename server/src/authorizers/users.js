const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param parent
 * @param args
 * @param {Schema.Context} ctx
 * @param info
 * @return {Promise<boolean>}
 */
async function isAuthenticatedRule(parent, args, ctx, info) {
  const userId = _.get(ctx, 'user.id');
  console.log(`\x1b[33m Rule (isAuthenticatedRule) userId ${userId} \x1b[0m`);
  return !!userId;
}

const isAuthenticated = rule()(isAuthenticatedRule);

module.exports = {
  isAuthenticated,
};
