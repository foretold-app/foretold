const _ = require('lodash');

const data = require('../data');

async function measurable(root, args, context, info) {
  const measurableId = args.measurableId || root.measurableId;
  if (!measurableId) return;
  const measurable = await data.measurablesData.getOne(measurableId);
  context.measurable = measurable;
  context.channelId = measurable.channelId;
}

module.exports = {
  measurable,
};
