const _ = require('lodash');

const data = require('../data');

async function measurable(root, args, context, info) {
  const measurableId = _.get(args, 'measurableId')
    || _.get(root, 'measurableId')
    || _.get(context, 'measurableId')
    || _.get(args, 'id');
  console.log('\x1b[36m ---> \x1b[0m Middleware (measurable)', { measurableId });
  if (!measurableId) return;
  const measurable = await data.measurablesData.getOne(measurableId);
  context.measurable = measurable;
  context.channelId = measurable.channelId;
}

module.exports = {
  measurable,
};
