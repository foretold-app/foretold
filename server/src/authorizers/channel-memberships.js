const _ = require('lodash');
const { rule } = require('graphql-shield');

const models = require('../models');

/**
 * @param {string} roleName
 */
function roleRule(roleName) {
  /**
   * @param {*} root
   * @param {object} args
   * @param {Schema.Context} context
   * @return {Promise<boolean>}
   */
  return (root, args, context) => {
    const role = _.get(context, 'channelMembershipsRole', []);
    console.log(`\x1b[33m Rule Channel Memberships (roleRule) ` +
      `role "${role}" = "${roleName}" \x1b[0m`);
    return role === roleName;
  };
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isInChannelRule(root, args, context, info) {
  const channelMembership = _.get(context, 'channelMembership');
  const agentId = _.get(context, 'agent.id');
  const result = !!channelMembership;
  console.log(`\x1b[33m Rule Channel Memberships (isInChannelRule) ` +
    `agentId ${agentId} = ${result} \x1b[0m`);
  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isOnlyOneAdminRule(root, args, context, info) {
  const channelMembershipsAdmins = _.get(context, 'channelMembershipsAdmins');
  const result =
    _.isArray(channelMembershipsAdmins) &&
    _.size(channelMembershipsAdmins) === 1;
  console.log(`\x1b[33m Rule Channel Memberships (onlyOneAdminRule) ` +
    `result = ${result} \x1b[0m`);
  return result;
}

const isAdminRule = roleRule(models.ChannelMemberships.ROLE.ADMIN);
const isViewerRule = roleRule(models.ChannelMemberships.ROLE.VIEWER);

/** @type {Rule} */
const isAdmin = rule({ cache: 'no_cache' })(isAdminRule);
/** @type {Rule} */
const isViewer = rule({ cache: 'no_cache' })(isViewerRule);
/** @type {Rule} */
const isInChannel = rule({ cache: 'no_cache' })(isInChannelRule);
/** @type {Rule} */
const isOnlyOneAdmin = rule({ cache: 'no_cache' })(isOnlyOneAdminRule);

module.exports = {
  isAdmin,
  isViewer,
  isInChannel,
  isOnlyOneAdmin,

  isAdminRule,
  isViewerRule,
};
