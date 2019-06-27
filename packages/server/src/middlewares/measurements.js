const _ = require('lodash');

const { MEASURABLE_STATE } = require('../models/measurable-state');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../models/measurement-competitor-type');

const MAX_XS = 1000;

const ERR_1 = () => 'You can only submit one type of value. ';
const ERR_2 = () => 'Xs and Ys should be the same size.';
const ERR_3 = (xsEntered) => `Xs of length (${xsEntered}) exceeds maximum of length ${MAX_XS}.`;
const ERR_4 = () => 'You must submit one kind of value.';
const ERR_5 = () => 'Measurable should be in an Open state.';

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
  const floatPoint = _.get(args, 'input.value.floatPoint');
  const percentage = _.get(args, 'input.value.percentage');
  const binary = _.get(args, 'input.value.binary');

  {
    const valuesEntered = [floatCdf, floatPoint, percentage, binary];
    const values = valuesEntered.filter(item => !!item);
    const countValues = values.length;

    if (countValues > 1) throw new Error(ERR_1());
    if (countValues === 0) throw new Error(ERR_4());
  }

  {
    const xs = _.get(floatCdf, 'xs');
    const ys = _.get(floatCdf, 'ys');
    const sizeXs = _.size(xs);
    const sizeYs = _.size(ys);

    if (floatCdf && sizeXs !== sizeYs) throw new Error(ERR_2());
    if (floatCdf && sizeXs > MAX_XS) throw new Error(ERR_3(sizeXs));
  }

  return true;
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} info
 * @return {Promise<boolean>}
 */
async function measurableStateValidation(root, args, context, info) {
  const measurementType = _.get(args, 'input.competitorType');
  const isCompetitive = MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE === measurementType;

  const measurableState = _.get(context, 'measurable.state', '');
  const isMeasurableAvailable = [
    MEASURABLE_STATE.OPEN,
    MEASURABLE_STATE.JUDGEMENT_PENDING,
  ].includes(measurableState);

  if (!isCompetitive) return true;
  if (!isMeasurableAvailable) throw new Error(ERR_5());

  return true;
}

module.exports = {
  measurableStateValidation,
  measurementValueValidation,
};
