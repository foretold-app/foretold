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
async function isMoreThenOneAdminRule(root, args, context, info) {
  const channelMembershipsAdmins = _.get(context, 'channelMembershipsAdmins');
  const result =
    _.isArray(channelMembershipsAdmins) &&
    _.size(channelMembershipsAdmins) > 1;
  console.log(`\x1b[33m Rule Channel Memberships (isMoreThenOneAdminRule) ` +
    `result = ${result} \x1b[0m`);
  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isSubjectAsObjectRule(root, args, context, info) {
  const objectAgentId = _.get(args, 'input.agentId')
    || _.get(root, 'agentId');
  const subjectAgentId = _.get(context, 'agent.id');
  const result = !!objectAgentId && objectAgentId === subjectAgentId;
  console.log(`\x1b[33m Rule Channel Memberships (isSubjectAsObjectRule) ` +
    `objectAgentId = ${objectAgentId} subjectAgentId = ${subjectAgentId} ` +
    `result = ${result} \x1b[0m`);
  return result;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function isObjectAdminRule(root, args, context, info) {
  const role = _.get(args, 'input.role')
    || _.get(root, 'role')
    || _.get(context, 'channelMembershipsRole');
  const result = !!role && role === CHANNEL_MEMBERSHIP_ROLES.ADMIN;
  console.log(`\x1b[33m Rule Channel Memberships (isObjectAdminRule) ` +
    `role = ${role} result = ${result} \x1b[0m`);
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
const isMoreThenOneAdmin = rule({ cache: 'no_cache' })(isMoreThenOneAdminRule);
/** @type {Rule} */
const isSubjectAsObject = rule({ cache: 'no_cache' })(isSubjectAsObjectRule);
/** @type {Rule} */
const isObjectAdmin = rule({ cache: 'no_cache' })(isObjectAdminRule);

module.exports = {
  isAdmin,
  isViewer,
  isInChannel,
  isMoreThenOneAdmin,
  isSubjectAsObject,
  isObjectAdmin,

  isAdminRule,
  isViewerRule,
};
