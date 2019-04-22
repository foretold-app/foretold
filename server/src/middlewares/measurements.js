const _ = require('lodash');

const MAX_XS = 1000;

const ERR_1 = 'Only FloatCdf or FloatPoint are available.';
const ERR_2 = 'Xs should be an array.';
const ERR_3 = 'Ys should be an array.';
const ERR_4 = 'Xs and Ys should have the same size.';
const ERR_5 = `Xs should have max size less then ${MAX_XS}.`;

/**
 * @param {*} root
 * @param {object} args
 * @param {object} args.input
 * @param {object} args.input.value
 * @param {object} args.input.value.floatCdf
 * @param {number[]} args.input.value.floatCdf.xs
 * @param {number[]} args.input.value.floatCdf.ys
 * @param {number} args.input.value.floatPoint
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function measurementValueValidation(root, args, context, info) {
  const floatCdf = _.get(args, 'input.value.floatCdf');
  const xs = _.get(floatCdf, 'xs');
  const ys = _.get(floatCdf, 'ys');

  const floatPoint = _.get(args, 'input.value.floatPoint');

  if (floatCdf !== undefined && floatPoint !== undefined) throw new Error(ERR_1);
  if (floatCdf !== undefined && !_.isArray(xs)) throw new Error(ERR_2);
  if (floatCdf !== undefined && !_.isArray(ys)) throw new Error(ERR_3);
  if (floatCdf !== undefined && _.size(xs) !== _.size(ys)) throw new Error(ERR_4);
  if (floatCdf !== undefined && _.size(xs) <= MAX_XS) throw new Error(ERR_5);

  return true;
}

module.exports = {
  measurementValueValidation,
};
