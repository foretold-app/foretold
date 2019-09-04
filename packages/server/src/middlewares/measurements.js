const _ = require('lodash');
const { MeasurementValue } = require('@foretold/measurement-value');

const { MEASURABLE_STATE } = require('../models/enums/measurable-state');
const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../models/enums/measurement-competitor-type');

// @todo: move to lang.js
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
  const value = _.get(args, ['input', 'value']);
  return MeasurementValue.factory(value).validate();
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
  const isCompetitive =
    MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE === measurementType;

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
