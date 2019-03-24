const data = require('../data');

/**
 *
 * @param next
 * @returns {function(*=, *=, *=, *=): *}
 */
function authorizerChannel(next) {
  return async (root, args, context, info) => {

    const channelId = args.channelId;
    const agentId = context.user.agentId;

    console.log('Channel authorizer', channelId);

    if (!channelId) {
      throw new Error('Channel ID is required');
    }

    const agentChannel = await data.agentsChannelsData.getOne({
      agentId,
      channelId,
    });

    if (!agentChannel) {
      throw new Error('Access denied');
    }

    return next(root, args, context, info);
  };
}


module.exports = {
  authorizerChannel,
};
