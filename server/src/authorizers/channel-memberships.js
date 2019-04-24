const _ = require('lodash');
const { rule } = require('graphql-shield');

const models = require('../models');
const { CHANNEL_MEMBERSHIP_ROLES } = require('../models/channel-membership-roles');

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
    const result = role === roleName;
    console.log(`\x1b[33m Rule Channel Memberships (roleRule) ` +
      `role "${role}" = "${roleName}", result = "${result}"\x1b[0m`);
    return result;
  };
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function channelHasMembershipWithCurrentAgentRule(root, args, context, info) {
  const channelMembership = _.get(context, 'channelMembership');
  const agentId = _.get(context, 'agent.id');
  const result = !!channelMembership;
  console.log(`\x1b[33m Rule Channel Memberships ` +
    `(channelHasMembershipWithCurrentAgentRule) ` +
    `agentId "${agentId}", result = "${result}"\x1b[0m`);
  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function channelHasMultipleAdminsRule(root, args, context, info) {
  const channelMembershipsAdmins = _.get(context, 'channelMembershipsAdmins');
  const result =
    _.isArray(channelMembershipsAdmins) &&
    _.size(channelMembershipsAdmins) > 1;
  console.log(`\x1b[33m Rule Channel Memberships ` +
    `(channelHasMultipleAdminsRule) result = "${result}"\x1b[0m`);
  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function membershipBelongsToCurrentAgentRule(root, args, context, info) {
  const objectAgentId = _.get(args, 'input.agentId')
    || _.get(root, 'agentId');
  const subjectAgentId = _.get(context, 'agent.id');
  const result = !!objectAgentId && objectAgentId === subjectAgentId;
  console.log(`\x1b[33m Rule Channel Memberships ` +
    `(membershipBelongsToCurrentAgentRule) objectAgentId = ` +
    `"${objectAgentId}", subjectAgentId = "${subjectAgentId}", ` +
    `result = "${result}"\x1b[0m`);
  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function membershipHasAdminRoleRule(root, args, context, info) {
  const role = _.get(args, 'input.role')
    || _.get(root, 'role')
    || _.get(context, 'channelMembershipsRole');
  const result = !!role && role === CHANNEL_MEMBERSHIP_ROLES.ADMIN;
  console.log(`\x1b[33m Rule Channel Memberships ` +
    `(membershipHasAdminRoleRule) ` +
    `role = "${role}", result = "${result}"\x1b[0m`);
  return result;
}

const isAdminRule = roleRule(models.ChannelMemberships.ROLE.ADMIN);
const isViewerRule = roleRule(models.ChannelMemberships.ROLE.VIEWER);

/** @type {Rule} */
const isAdmin = rule({
  cache: 'no_cache',
})(isAdminRule);

/** @type {Rule} */
const isViewer = rule({
  cache: 'no_cache',
})(isViewerRule);

/** @type {Rule} */
const channelHasMembershipWithCurrentAgent = rule({
  cache: 'no_cache',
})(channelHasMembershipWithCurrentAgentRule);

/** @type {Rule} */
const channelHasMultipleAdmins = rule({
  cache: 'no_cache',
})(channelHasMultipleAdminsRule);

/** @type {Rule} */
const membershipBelongsToCurrentAgent = rule({
  cache: 'no_cache'
})(membershipBelongsToCurrentAgentRule);

/** @type {Rule} */
const membershipHasAdminRole = rule({
  cache: 'no_cache'
})(membershipHasAdminRoleRule);

module.exports = {
  isAdmin,
  isViewer,
  channelHasMembershipWithCurrentAgent,
  channelHasMultipleAdmins,
  membershipBelongsToCurrentAgent,
  membershipHasAdminRole,

  isAdminRule,
  isViewerRule,
};
