var Finance = require('financejs')
import math from 'mathjs'
import _ from "lodash";

import {Filters, SAMPLE_FILTERED} from './filters/filters'
import {ImpureConstructs} from './constructs/constructs'
import {Distributions} from "./distributions/distributions"
import {Multimodals} from './multimodals/multimodals';

const finance = new Finance()

const SAMPLING_ERROR = 1;
const PARSER_ERROR = 1;
const FUNCTIONS_CONTAIN_UNITS_ERROR = 1;
const UNEXPECTED_END_OF_EXPRESSION_ERROR = 1;
const DIVIDE_BY_ZERO_ERROR = 1;
const ALL_SAMPLES_FILTERED_ERROR = 1;

const financeFunctions = {
  PV: finance.PV,
  FV: finance.FV,
  NPV: finance.NPV,
  //IRR: finance.IRR, Too slow.
  PP: finance.PP,
  ROI: finance.ROI,
  AM: finance.AM,
  PI: finance.PI,
  DF: finance.DF,
  CI: finance.CI,
  CAGR: finance.CAGR,
  LR: finance.LR,
  R72: finance.R72,
  WACC: finance.WACC
}

// Distributions:
math.import(Distributions, {override: true})
// Financial functions:
math.import(financeFunctions, {override: true})
// Guesstimate constructs:
math.import(ImpureConstructs, {override: true, wrap: true})
// Filters
math.import(Filters, {override: true})
// Multimodals
math.import(Multimodals, {override: true})

// All of jStat's functions are impure as they require sampling on pure inputs.
export const STOCHASTIC_FUNCTIONS = ['pickRandom', 'randomInt', 'random'].concat(Object.keys(Distributions)).concat(Object.keys(ImpureConstructs)).concat(Object.keys(Multimodals))

export function Evaluate(text, sampleCount, inputs) {
  try {
    const compiled = math.compile(text)

    return evaluate(compiled, inputs, sampleCount, text)
  } catch ({message}) {
    if (message.startsWith('Unexpected end of expression')) {
      return {errors: [{type: SAMPLING_ERROR, subType: UNEXPECTED_END_OF_EXPRESSION_ERROR, rawMessage: message}]}
    } else {
      return {errors: [{type: SAMPLING_ERROR, rawMessage: message}]}
    }
  }
}

function sampleInputs(inputs, i) {
  const sample = {}
  for (let key of Object.keys(inputs)){
    sample[key] = inputs[key][i % inputs[key].length]
    util.inspect(inputs)
  }
  return sample
}

function evaluate(compiled, inputs, n, text){
  let values = []
  let errors = []
  let anyNotFiltered = false

  for (var i = 0; i < n; i++) {
    const sampledInputs = sampleInputs(inputs, i)
    const someInputFiltered = _.some(sampledInputs, val => _.isEqual(val, SAMPLE_FILTERED))
    let newSample = NaN
    try {
      newSample = someInputFiltered ? SAMPLE_FILTERED : compiled.eval(sampledInputs)

    } catch (rawError) {
      const isUnexpectedTypeError = rawError.message.includes('Unexpected type of argument in function')
      const containsFilterFn = _.some(Object.keys(Filters), f => text.includes(f))
      if (isUnexpectedTypeError && containsFilterFn) {
        newSample = SAMPLE_FILTERED
      } else {
        return {values: [], errors: [{type: SAMPLING_ERROR, rawError}]}
      }
    }

    if (_.isFinite(newSample)) {
      anyNotFiltered = true
      values.push(newSample)
    } else if (newSample === SAMPLE_FILTERED) {
      values.push(newSample)
    } else if ([Infinity, -Infinity].includes(newSample)) {
      errors.push({type: SAMPLING_ERROR, subType: DIVIDE_BY_ZERO_ERROR})
      values.push(newSample)
    } else if (newSample.constructor.name === 'Unit') {
      return {values: [], errors: [{type: PARSER_ERROR, subType: FUNCTIONS_CONTAIN_UNITS_ERROR}]}
    } else if (typeof newSample === 'function') {
      return {values: [], errors: [{type: PARSER_ERROR, subType: INCOMPLETE_FUNCTION_ERROR}]}
    } else {
      if (__DEV__) { console.warn('Unidentified sample detected: ', newSample) }
      return {values: [], errors: [{type: SAMPLING_ERROR}]}
    }
  }

  errors = _.uniq(errors)

  return anyNotFiltered ? {values, errors} : {values: [], errors: [...errors, {type: SAMPLING_ERROR, subType: ALL_SAMPLES_FILTERED_ERROR}]}
}
