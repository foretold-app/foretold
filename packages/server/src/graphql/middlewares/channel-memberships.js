const _ = require('lodash');

const { ChannelMembershipsData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

const log = logger.module('middlewares/channel-memberships');

/**
 * @todo: Should be divided on two middleware.
 * @todo: To fix "||" a joined logic.
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextChannelMemberships(root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null)
    || _.get(args, 'input.channelId', null)
    || _.get(root, 'channelId', null)
    || _.get(context, 'channelId', null)
    || _.get(context, 'measurable.channelId', null)
    || _.get(context, 'series.channelId', null)
    || _.get(context, 'channel.id', null);
  const agentId = _.get(context, 'agent.id', null);

  const compoundId = { agentId, channelId };

  log.trace('\x1b[36m ---> \x1b[0m Middleware '
    + '(setContextChannelMemberships)', compoundId);

  if (!!channelId && !!agentId) {
    const params = new Params(compoundId);
    const query = new Query();
    const options = new Options({ raw: true });
    const channelMembership = await new ChannelMembershipsData().getOne(
      params, query, options,
    );
    context.channelMembership = channelMembership;
    context.channelMembershipsRole = _.get(channelMembership, 'role', null);
  } else {
    context.channelMembership = null;
    context.channelMembershipsRole = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextBotOwnerChannelMemberships(
  root, args, context, _info,
) {
  const channelId = _.get(args, 'channelId', null)
    || _.get(args, 'input.channelId', null)
    || _.get(root, 'channelId', null)
    || _.get(context, 'channelId', null)
    || _.get(context, 'measurable.channelId', null)
    || _.get(context, 'channel.id', null);
  const agentId = _.get(context, 'botUserOwner.agentId', null);

  const compoundId = { agentId, channelId };

  log.trace('\x1b[36m ---> \x1b[0m Middleware '
    + '(setContextBotOwnerChannelMemberships)', compoundId);

  if (!!channelId && !!agentId) {
    const params = new Params(compoundId);
    const query = new Query();
    const options = new Options({ raw: true });
    const channelMembership = await new ChannelMembershipsData().getOne(
      params, query, options,
    );
    context.botUserOwnerChannelMembership = channelMembership;
    context.botUserOwnerChannelMembershipsRole = _.get(
      channelMembership, 'role', null,
    );
  } else {
    context.botUserOwnerChannelMembership = null;
    context.botUserOwnerChannelMembershipsRole = null;
  }
}

/**
 * @todo: To fix "||" a joined logic.
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextChannelMembershipsAdmins(root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null)
    || _.get(args, 'input.channelId', null)
    || _.get(root, 'channelId', null)
    || _.get(context, 'channelId', null)
    || _.get(context, 'measurable.channelId', null)
    || _.get(context, 'channel.id', null);

  log.trace('\x1b[36m ---> \x1b[0m Middleware '
    + '(setContextChannelMembershipsAdmins)', channelId);

  if (!!channelId) {
    context.channelMembershipsAdmins = await new ChannelMembershipsData()
      .getAllOnlyAdmins(channelId);
  } else {
    context.channelMembershipsAdmins = null;
  }
}

module.exports = {
  setContextChannelMemberships,
  setContextChannelMembershipsAdmins,
  setContextBotOwnerChannelMemberships,
};
