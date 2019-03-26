const _ = require('lodash');

const data = require('../data');

/**
 * @tested
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function serie(root, args, context, info) {
  const id = _.get(args, 'id');
  console.log('\x1b[36m ---> \x1b[0m Middleware (serie)', { id });
  if (!id) return;
  const serie = await data.seriesData.getOne(id);
  context.serie = serie;
  context.channelId = serie.channelId;
}

module.exports = {
  serie,
};
