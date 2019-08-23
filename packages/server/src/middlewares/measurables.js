const _ = require('lodash');

const data = require('../data');
const { Params } = require('../data/classes/params');

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

  console.log('\x1b[36m ---> \x1b[0m Middleware (measurable)', { measurableId });

  if (measurableId) {
    const params = new Params({ id: measurableId });
    context.measurable = await data.measurables.getOne(params);
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
  console.log('\x1b[36m ---> \x1b[0m Middleware (measurableByRoot)');
  context.measurable = root || null;
}

module.exports = {
  setContextMeasurable,
  setContextMeasurableByRoot,
};
