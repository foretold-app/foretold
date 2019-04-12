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
  const result = !!userId;
  console.log(`\x1b[33m Rule Users (isAuthenticatedRule) ` +
    `userId ${userId} = ${result} \x1b[0m`);
  return result;
}

/** @type {Rule} */
const isAuthenticated = rule()(isAuthenticatedRule);

module.exports = {
  isAuthenticated,
  isAuthenticatedRule,
};
