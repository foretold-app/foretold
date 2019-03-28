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

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isOwnerRule(root, args, context, info) {
  const roles = _.get(context, 'agentsChannelsRoles', []);
  console.log(`\x1b[33m Rule (isOwner) roles ${roles.join(', ')} \x1b[0m`);
  return roles.includes('owner');
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isViewerRule(root, args, context, info) {
  const roles = _.get(context, 'agentsChannelsRoles', []);
  console.log(`\x1b[33m Rule (isViewer) roles ${roles.join(', ')} \x1b[0m`);
  return roles.includes('viewer');
}

/** @type {Rule} */
const isAdmin = rule()(isAdminRule);

/** @type {Rule} */
const isOwner = rule()(isOwnerRule);

/** @type {Rule} */
const isViewer = rule()(isViewerRule);

module.exports = {
  isAdmin,
  isOwner,
  isViewer,
};
