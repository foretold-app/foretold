const _ = require('lodash');

const { MeasurablesData } = require('../../data');

const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');
const logger = require('../../lib/log');

const log = logger.module('middlewares/measurables');

/**
 * @todo: To fix "||" a joined logic.
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
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

  if (!!measurableId) {
    const params = new Params({ id: measurableId });
    const query = new Query();
    const options = new Options({ raw: true });
    context.measurable = await new MeasurablesData().getOne(
      params, query, options,
    );
  } else {
    context.measurable = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextMeasurableByRoot(root, args, context, _info) {
  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextMeasurableByRoot)');
  context.measurable = root || null;
}

module.exports = {
  setContextMeasurable,
  setContextMeasurableByRoot,
};
