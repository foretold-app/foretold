const _ = require('lodash');
const { rule } = require('graphql-shield');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isAdminRule(root, args, context, info) {
  const roles = _.get(context, 'agentsChannelsRoles', []);
  console.log(`\x1b[33m Rule (isAdmin) roles ${roles.join(', ')} \x1b[0m`);
  return roles.includes('admin');
}

/** @type {Rule} */
const isAdmin = rule()(isAdminRule);

module.exports = {
  isAdmin,
  isAdminRule,
};
