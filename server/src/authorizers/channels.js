const _ = require('lodash');

const data = require('../data');

/**
 * @param {string} channelId
 * @param {string} agentId
 * @returns {Promise<boolean>}
 */
async function channel(channelId, agentId) {
  if (!channelId) {
    return Promise.reject(new Error('Channel ID is required'));
  }

  if (!agentId) {
    return Promise.reject(new Error('Agent ID is required'));
  }

  const channel = await data.channelsData.getOne(channelId);
  if (channel.isPublic) {
    return true;
  }

  const agentChannel = await data.agentsChannelsData.getOne({ agentId, channelId });
  if (!agentChannel) {
    return Promise.reject(new Error('Access denied'));
  }

  return true;
}

/**
 * @param next
 * @returns {function(*=, *=, *=, *=): *}
 */
function authorizerChannelByArg(next) {
  return async (root, args, context, info) => {
    const channelId = args.channelId;
    const agentId = context.user.agentId;

    console.log('Channel authorizer', channelId);
    if (await channel(channelId, agentId)) {
      return next(root, args, context, info);
    }
    return null;
  };
}

/**
 * @param next
 * @returns {function(*=, *=, *=, *=): *}
 */
function authorizerChannelByArgId(next) {
  return async (root, args, context, info) => {
    const channelId = args.id;
    const agentId = context.user.agentId;

    console.log('Channel authorizer', channelId);
    if (await channel(channelId, agentId)) {
      return next(root, args, context, info);
    }
    return null;
  };
}

/**
 * @param next
 * @returns {function(*=, *=, *=, *=): *}
 */
function authorizerChannelAfter(next) {
  return async (root, args, context, info) => {
    const entity = await next(root, args, context, info);
    const channelId = _.get(entity, 'channelId');
    const agentId = _.get(context, 'user.agentId');

    console.log('Channel authorizer', channelId);
    if (await channel(channelId, agentId)) {
      return entity;
    }
    return null;
  };
}

/**
 * @param next
 * @returns {function(*=, *=, *=, *=): *}
 */
function authorizerChannelAfterByCtx(next) {
  return async (root, args, context, info) => {
    const entity = await next(root, args, context, info);
    const channelId = _.get(context, 'channelId');
    const agentId = _.get(context, 'user.agentId');

    console.log('Channel authorizer by context', channelId);
    if (await channel(channelId, agentId)) {
      return entity;
    }
    return null;
  };
}

module.exports = {
  authorizerChannelByArg,
  authorizerChannelByArgId,
  authorizerChannelAfter,
  authorizerChannelAfterByCtx
};
