const _ = require('lodash');

const { MeasurementsData } = require('../../data');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');
const { Options } = require('../../data/classes');
const logger = require('../../lib/log');

const log = logger.module('middlewares/measurement');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextMeasurement(root, args, context, _info) {
  const measurementId = _.get(args, 'measurementId', null);

  log.trace(
    '\x1b[36m ---> \x1b[0m Middleware (setContextMeasurement)',
    { measurementId },
  );

  if (!!measurementId) {
    const params = new Params({ id: measurementId });
    const query = new Query();
    const options = new Options({ raw: true });
    context.measurement = await new MeasurementsData().getOne(
      params, query, options,
    );
  } else {
    context.measurement = null;
  }
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @return {Promise<void>}
 */
async function setContextMeasurementByRoot(root, args, context, _info) {
  log.trace('\x1b[36m ---> \x1b[0m Middleware (setContextMeasurementByRoot)');
  context.measurement = root || null;
}

module.exports = {
  setContextMeasurement,
  setContextMeasurementByRoot,
};
