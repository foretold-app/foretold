const _ = require('lodash');

const data = require('../../data');
const logger = require('../../lib/log');

const log = logger.module('middlewares/channel-memberships');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function setContextChannelMemberships(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(args, 'input.channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId')
    || _.get(context, 'measurable.channelId')
    || _.get(context, 'channel.id');
  const agentId = _.get(context, 'agent.id');

  const compoundId = { agentId, channelId };
  log.trace('\x1b[36m ---> \x1b[0m Middleware '
    + '(setContextChannelMemberships)', compoundId);

  if (!!channelId && !!agentId) {
    const channelMembership = await data.channelMemberships.getOne(compoundId);
    context.channelMembership = channelMembership;
    context.channelMembershipsRole = _.get(channelMembership, 'role');
  } else {
    context.channelMembership = null;
    context.channelMembershipsRole = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function setContextChannelMembershipsAdmins(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(args, 'input.channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId')
    || _.get(context, 'measurable.channelId')
    || _.get(context, 'channel.id');

  log.trace('\x1b[36m ---> \x1b[0m Middleware '
    + '(channelMembershipsAdmins)', channelId);

  if (!!channelId) {
    context.channelMembershipsAdmins
      = await data.channelMemberships.getAllOnlyAdmins({ channelId });
  } else {
    context.channelMembershipsAdmins = null;
  }
}

module.exports = {
  setContextChannelMemberships,
  setContextChannelMembershipsAdmins,
};
