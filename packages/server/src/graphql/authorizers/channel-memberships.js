const _ = require('lodash');
const { rule } = require('graphql-shield');

const models = require('../../models');
const { CHANNEL_MEMBERSHIP_ROLES } = require('../../enums/channel-membership-roles');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @return {boolean}
 */
function currentAgentIsChannelAdminRule(root, args, context) {
  const roleName = models.ChannelMemberships.ROLE.ADMIN;
  const role = _.get(context, 'channelMembershipsRole');

  const result =
    (!!role && !!roleName) &&
    (role === roleName);

  console.log(
    `\x1b[33m Rule Channel Memberships (currentAgentIsChannelAdminRule) ` +
    `role "${role}" = "${roleName}", result = "${result}".\x1b[0m`
  );

  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @return {boolean}
 */
function currentAgentIsChannelViewerRule(root, args, context) {
  const roleName = models.ChannelMemberships.ROLE.VIEWER;
  const role = _.get(context, 'channelMembershipsRole');

  const result =
    (!!role && !!roleName) &&
    (role === roleName);

  console.log(
    `\x1b[33m Rule Channel Memberships (currentAgentIsChannelViewerRule) ` +
    `role "${role}" = "${roleName}", result = "${result}".\x1b[0m`
  );

  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {boolean}
 */
function channelHasMembershipWithCurrentAgentRule(root, args, context, info) {
  const channelMembership = _.get(context, 'channelMembership');
  const agentId = _.get(context, 'agent.id');

  const result = !!channelMembership;

  console.log(
    `\x1b[33m Rule Channel Memberships ` +
    `(channelHasMembershipWithCurrentAgentRule) ` +
    `agentId "${agentId}", result = "${result}".\x1b[0m`
  );

  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {boolean}
 */
function channelHasMultipleAdminsRule(root, args, context, info) {
  const channelMembershipsAdmins = _.get(context, 'channelMembershipsAdmins');

  const result =
    _.isArray(channelMembershipsAdmins) &&
    _.size(channelMembershipsAdmins) > 1;

  console.log(
    `\x1b[33m Rule Channel Memberships ` +
    `(channelHasMultipleAdminsRule) result = "${result}".\x1b[0m`
  );

  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {boolean}
 */
function membershipBelongsToCurrentAgentRule(root, args, context, info) {
  const objectAgentId = _.get(args, 'input.agentId')
    || _.get(root, 'agentId');
  const subjectAgentId = _.get(context, 'agent.id');

  const result = !!objectAgentId && objectAgentId === subjectAgentId;

  console.log(
    `\x1b[33m Rule Channel Memberships ` +
    `(membershipBelongsToCurrentAgentRule) objectAgentId = ` +
    `"${objectAgentId}", subjectAgentId = "${subjectAgentId}", ` +
    `result = "${result}".\x1b[0m`
  );

  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {boolean}
 */
function membershipHasAdminRoleRule(root, args, context, info) {
  const role = _.get(args, 'input.role')
    || _.get(root, 'role')
    || _.get(context, 'channelMembershipsRole');

  const result = !!role && role === CHANNEL_MEMBERSHIP_ROLES.ADMIN;

  console.log(
    `\x1b[33m Rule Channel Memberships ` +
    `(membershipHasAdminRoleRule) ` +
    `role = "${role}", result = "${result}".\x1b[0m`
  );

  return result;
}

/** @type {Rule} */
const currentAgentIsChannelAdmin = rule({
  cache: 'no_cache',
})(currentAgentIsChannelAdminRule);

/** @type {Rule} */
const currentAgentIsChannelViewer = rule({
  cache: 'no_cache',
})(currentAgentIsChannelViewerRule);

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
  currentAgentIsChannelAdmin,
  currentAgentIsChannelViewer,
  channelHasMembershipWithCurrentAgent,
  channelHasMultipleAdmins,
  membershipBelongsToCurrentAgent,
  membershipHasAdminRole,
};
