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
    console.log('Middleware (after) measurable, measurableId', measurableId);
    const measurable = await data.measurablesData.getOne(measurableId);
    context.measurable = measurable;
    context.channelId = measurable.channelId;
    return entity;
  };
}

/**
 * @param next
 * @returns {function(*=, *=, *=, *=): *}
 */
function middlewareMeasurableBeforeByArg(next) {
  return async (root, args, context, info) => {
    const measurableId = _.get(args, 'measurableId');
    console.log('Middleware (before) measurable, measurableId', measurableId);
    const measurable = await data.measurablesData.getOne(measurableId);
    context.measurable = measurable;
    context.channelId = measurable.channelId;
    return next(root, args, context, info);
  };
}

module.exports = {
  middlewareMeasurableBeforeByArg,
  middlewareMeasurableAfter,
};
