const _ = require('lodash');

const { channelDoesNotExists } = require('../../../config/lang');

const { ChannelsData } = require('../../data');
const logger = require('../../lib/log');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');

const log = logger.module('middlewares/channels');

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function channelExistsValidation(root, args, _context, _info) {
  const channelId = _.get(args, 'channelId', null);

  const params = new Params({ id: channelId });
  const query = new Query();
  const options = new Options({ raw: true });
  const count = await new ChannelsData().getCount(params, query, options);

  if (!count) throw new Error(channelDoesNotExists());

  return true;
}

/**
 * @todo: To fix "||" a joined logic.
 * @param {object | null} root
 * @param {{ channelId: string }} args
 * @param {Schema.Context} context
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

  if (!!channelId) {
    const params = new Params({ id: channelId });
    context.channel = await new ChannelsData().getOne(params);
  } else {
    context.channel = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextChannelByRoot(root, args, context, _info) {
  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextChannelByRoot)');
  context.channel = !!root ? root : null;
}

module.exports = {
  channelExistsValidation,

  setContextChannel,
  setContextChannelByRoot,
};
