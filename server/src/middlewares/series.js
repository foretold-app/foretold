const _ = require('lodash');

const data = require('../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function series(root, args, context, info) {
  const id = _.get(args, 'id');
  console.log('\x1b[36m ---> \x1b[0m Middleware (series)', { id });
  if (!id) return;
  const series = await data.seriesData.getOne(id);
  context.series = series;
  context.channelId = series.channelId;
}

module.exports = {
  series,
};
