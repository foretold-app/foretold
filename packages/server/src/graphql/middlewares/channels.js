const _ = require('lodash');

const { ChannelsData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');

const log = logger.module('middlewares/channels');

/**
 * @todo: To fix "||" a joined logic.
 * @param {object | null} root
 * @param {{ channelId: string }} args
 * @param {object} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextChannel(root, args, context, _info) {
  const channelId = _.get(args, 'channelId', null)
    || _.get(args, 'input.channelId', null)
    || _.get(root, 'channelId', null)
    || _.get(context, 'channelId', null)
    || _.get(context, 'measurable.channelId', null)
    || _.get(args, 'id', null);

  log.trace(
    '\x1b[36m ---> \x1b[0m Middleware (setContextChannel)',
    { channelId },
  );

  if (channelId) {
    context.channel = await new ChannelsData().getOne(new Params({
      id: channelId,
    }));
  } else {
    context.channel = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextChannelByRoot(root, args, context, _info) {
  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextChannelByRoot)');
  context.channel = !!root ? root : null;
}

module.exports = {
  setContextChannel,
  setContextChannelByRoot,
};
