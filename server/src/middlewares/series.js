const _ = require('lodash');

const data = require('../data');

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
