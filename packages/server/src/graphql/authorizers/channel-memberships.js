const _ = require('lodash');
const { rule } = require('graphql-shield');

const models = require('../../data/models/definitions');
const { CHANNEL_MEMBERSHIP_ROLES } = require('../../enums');
const logger = require('../../lib/log');

const log = logger.module('authorizers/channel-memberships');

/**
 * @param {*} _root
 * @param {object} args
 * @param {Schema.Context} context
 * @return {boolean}
 */
function currentAgentIsChannelAdminRule(_root, args, context) {
  const roleName = models.ChannelMemberships.ROLE.ADMIN;
  const role = _.get(context, 'channelMembershipsRole', null);

  const result = (!!role && !!roleName)
    && (role === roleName);

  log.trace(
    '\x1b[33m Rule Channel Memberships (currentAgentIsChannelAdminRule) '
    + `role "${role}" = "${roleName}", result = "${result}".\x1b[0m`,
  );

  return result;
}

/**
 * @param {*} _root
 * @param {object} args
 * @param {Schema.Context} context
 * @return {boolean}
 */
function currentAgentIsChannelViewerRule(_root, args, context) {
  const roleName = models.ChannelMemberships.ROLE.VIEWER;
  const role = _.get(context, 'channelMembershipsRole', null);

  const result = (!!role && !!roleName)
    && (role === roleName);

  log.trace(
    '\x1b[33m Rule Channel Memberships (currentAgentIsChannelViewerRule) '
    + `role "${role}" = "${roleName}", result = "${result}".\x1b[0m`,
  );

  return result;
}

/**
 * @param {*} _root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function channelHasMembershipWithCurrentAgentRule(
  _root,
  _args,
  context,
  _info,
) {
  const channelMembership = _.get(context, 'channelMembership', null);
  const agentId = _.get(context, 'agent.id', null);

  const result = !!channelMembership;

  log.trace(
    '\x1b[33m Rule Channel Memberships '
    + '(channelHasMembershipWithCurrentAgentRule) '
    + `agentId "${agentId}", result = "${result}".\x1b[0m`,
  );

  return result;
}

/**
 * @param {*} _root
 * @param {object} _args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function channelHasMultipleAdminsRule(_root, _args, context, _info) {
  const channelMembershipsAdmins = _.get(
    context, 'channelMembershipsAdmins', null,
  );

  const result = _.isArray(channelMembershipsAdmins)
    && _.size(channelMembershipsAdmins) > 1;

  log.trace(
    '\x1b[33m Rule Channel Memberships '
    + `(channelHasMultipleAdminsRule) result = "${result}".\x1b[0m`,
  );

  return result;
}

/**
 * @todo: To fix "_.get(root, 'agentId')".
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function membershipBelongsToCurrentAgentRule(root, args, context, _info) {
  const membershipAgentId = _.get(args, 'input.agentId', null)
    || _.get(root, 'agentId', null);
  const currentAgentId = _.get(context, 'agent.id', null);

  const result = (!!membershipAgentId && !!currentAgentId )
    && membershipAgentId === currentAgentId;

  log.trace(
    '\x1b[33m Rule Channel Memberships '
    + '(membershipBelongsToCurrentAgentRule) membershipAgentId = '
    + `"${membershipAgentId}", currentAgentId = "${currentAgentId}", `
    + `result = "${result}".\x1b[0m`,
  );

  return result;
}

/**
 * @todo: To fix "_.get(root, 'role')".
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {boolean}
 */
function membershipHasAdminRoleRule(root, args, context, _info) {
  const role = _.get(args, 'input.role', null)
    || _.get(root, 'role', null)
    || _.get(context, 'channelMembershipsRole', null);

  const result = !!role && role === CHANNEL_MEMBERSHIP_ROLES.ADMIN;

  log.trace(
    '\x1b[33m Rule Channel Memberships '
    + '(membershipHasAdminRoleRule) '
    + `role = "${role}", result = "${result}".\x1b[0m`,
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
  cache: 'no_cache',
})(membershipBelongsToCurrentAgentRule);

/** @type {Rule} */
const membershipHasAdminRole = rule({
  cache: 'no_cache',
})(membershipHasAdminRoleRule);

module.exports = {
  currentAgentIsChannelAdmin,
  currentAgentIsChannelViewer,
  channelHasMembershipWithCurrentAgent,
  channelHasMultipleAdmins,
  membershipBelongsToCurrentAgent,
  membershipHasAdminRole,
};
