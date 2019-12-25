const _ = require('lodash');

const { MeasurablesData } = require('../../data');
const { Params } = require('../../data/classes');
const { measurableEmptyName } = require('../../../config/lang');
const logger = require('../../lib/log');

const log = logger.module('middlewares/measurables');

/**
 * @todo: To fix "||" a joined logic.
 * @param {object | null} root
 * @param {object} args
 * @param {object} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextMeasurable(root, args, context, _info) {
  const measurableId = _.get(args, 'measurableId', null)
    || _.get(args, 'input.measurableId', null)
    || _.get(root, 'measurableId', null)
    || _.get(context, 'measurableId', null)
    || _.get(args, 'id', null);

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
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextMeasurableByRoot(root, args, context, _info) {
  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextMeasurableByRoot)');
  context.measurable = root || null;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} _context
 * @param {object} _info
 * @return {Promise<boolean>}
 */
async function measurableNameValidation(root, args, _context, _info) {
  const name = _.get(args, 'input.name', null);
  const labelSubject = _.get(args, 'input.labelSubject', null);
  const isName = !!name || !!labelSubject;

  if (!isName) throw new Error(measurableEmptyName());

  return true;
}

module.exports = {
  measurableNameValidation,
  setContextMeasurable,
  setContextMeasurableByRoot,
};
