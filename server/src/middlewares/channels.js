const _ = require('lodash');

const data = require('../data');

/**
 * @param {object | null} root
 * @param {{ channelId: string }} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function channel(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(args, 'input.channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId')
    || _.get(context, 'measurable.channelId')
    || _.get(args, 'id');

  console.log('\x1b[36m ---> \x1b[0m Middleware (channel)', { channelId });
  context.channel = channelId
    ? await data.channels.getOne(channelId)
    : null;
}

module.exports = {
  channel,
};
