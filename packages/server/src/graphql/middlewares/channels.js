const _ = require('lodash');

const data = require('../../data');

/**
 * @param {object | null} root
 * @param {{ channelId: string }} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function setContextChannel(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(args, 'input.channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId')
    || _.get(context, 'measurable.channelId')
    || _.get(args, 'id');

  console.log(
    '\x1b[36m ---> \x1b[0m Middleware (setContextChannel)',
    { channelId },
  );

  if (channelId) {
    context.channel = await data.channels.getOne({ id: channelId });
  } else {
    context.channel = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function setContextChannelByRoot(root, args, context, info) {
  console.log('\x1b[36m ---> \x1b[0m Middleware (setContextChannelByRoot)');
  context.channel = !!root ? root : null;
}

module.exports = {
  setContextChannel,
  setContextChannelByRoot,
};
