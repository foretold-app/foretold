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
  console.log(`\x1b[33m Rule (isAdminRule) roles "${roles.join(', ')}" \x1b[0m`);
  return roles.includes('admin');
}

/** @type {Rule} */
const isAdmin = rule()(isAdminRule);

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isViewerRule(root, args, context, info) {
  const roles = _.get(context, 'agentsChannelsRoles', []);
  console.log(`\x1b[33m Rule (isViewerRule) roles "${roles.join(', ')}" \x1b[0m`);
  return roles.includes('viewer');
}

/** @type {Rule} */
const isViewer = rule()(isViewerRule);

module.exports = {
  isAdmin,
  isAdminRule,
  isViewer,
};
