const _ = require('lodash');

const data = require('../data');

async function channel(root, args, context, info) {
  const channelId = args.channelId || root.channelId || context.channelId;
  context.channel = channelId
    ? await data.channelsData.getOne(channelId)
    : null;
}

module.exports = {
  channel,
};
