const _ = require('lodash');

const { MeasurablesData } = require('../../data');
const { Params } = require('../../data/classes');
const { measurableEmptyName } = require('../../lang');
const logger = require('../../lib/log');

const log = logger.module('middlewares/measurables');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function setContextMeasurable(root, args, context, info) {
  const measurableId = _.get(args, 'measurableId')
    || _.get(args, 'input.measurableId')
    || _.get(root, 'measurableId')
    || _.get(context, 'measurableId')
    || _.get(args, 'id');

  log.trace(
    '\x1b[36m ---> \x1b[0m Middleware (setContextMeasurable)',
    { measurableId },
  );

  if (measurableId) {
    const params = new Params({ id: measurableId });
    context.measurable = await new MeasurablesData().getOne(params);
  } else {
    context.measurable = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} info
 * @return {Promise<void>}
 */
async function setContextMeasurableByRoot(root, args, context, info) {
  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextMeasurableByRoot)');
  context.measurable = root || null;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function measurableNameValidation(root, args, context, info) {
  const name = _.get(args, 'input.name');
  const labelSubject = _.get(args, 'input.labelSubject');
  const isName = !!name || !!labelSubject;

  if (!isName) throw new Error(measurableEmptyName());

  return true;
}

module.exports = {
  measurableNameValidation,
  setContextMeasurable,
  setContextMeasurableByRoot,
};
