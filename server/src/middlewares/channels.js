const _ = require('lodash');

const data = require('../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function channel(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId');
  console.log('\x1b[36m ---> \x1b[0m Middleware (channel)', { channelId });
  context.channel = channelId
    ? await data.channelsData.getOne(channelId)
    : null;
}

module.exports = {
  channel,
};
