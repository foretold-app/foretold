const _ = require('lodash');

const data = require('../data');

/**
 * @param next
 * @returns {function(*=, *=, *=, *=): *}
 */
function middlewareMeasurableAfter(next) {
  return async (root, args, context, info) => {
    const entity = await next(root, args, context, info);
    const measurableId = _.get(entity, 'measurableId');
    console.log('Middleware measurable, measurableId', measurableId);
    const measurable = await data.measurablesData.getOne(measurableId);
    context.measurable = measurable;
    context.channelId = measurable.channelId;
    return entity;
  };
}

module.exports = {
  middlewareMeasurableAfter,
};
