const _ = require('lodash');

const data = require('../data');

/**
 * @todo: rename "setContextChannelMemberships"
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function channelMemberships(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(args, 'input.channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId')
    || _.get(context, 'measurable.channelId')
    || _.get(context, 'channel.id');
  const agentId = _.get(context, 'agent.id');

  const compoundId = { agentId, channelId };
  console.log('\x1b[36m ---> \x1b[0m Middleware ' +
    '(channelMemberships)', compoundId);

  if (channelId && agentId) {
    const channelMembership = await data.channelMemberships.getOne(compoundId);
    context.channelMembership = channelMembership;
    context.channelMembershipsRole = _.get(channelMembership, 'role');
  } else {
    context.channelMembership = null;
    context.channelMembershipsRole = null;
  }
}

/**
 * @todo: rename "setContextChannelMembershipsAdmins"
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function channelMembershipsAdmins(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(args, 'input.channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId')
    || _.get(context, 'measurable.channelId')
    || _.get(context, 'channel.id');

  console.log('\x1b[36m ---> \x1b[0m Middleware ' +
    '(channelMembershipsAdmins)', channelId);

  if (channelId) {
    context.channelMembershipsAdmins =
      await data.channelMemberships.getAllOnlyAdmins({
        channelId,
      });
  } else {
    context.channelMembershipsAdmins = null;
  }
}

module.exports = {
  channelMemberships,
  channelMembershipsAdmins,
};
