const _ = require('lodash');

const { ChannelBookmarksData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');

const log = logger.module('middlewares/channel-memberships');

/**
 * @todo: To fix "||" a joined logic.
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextChannelBookmark(root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null);
  const agentId = _.get(context, 'agent.id', null);

  log.trace('\x1b[36m ---> \x1b[0m Middleware '
    + '(setContextChannelBookmark)', { agentId, channelId });

  if (!!channelId && !!agentId) {
    const params = new Params({ agentId, channelId });
    const channelBookmark = await new ChannelBookmarksData().getOne(params);
    context.channelBookmark = channelBookmark;
  } else {
    context.channelBookmark = null;
  }
}

module.exports = {
  setContextChannelBookmark,
};
