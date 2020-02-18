import {
  Evaluate,
  STOCHASTIC_FUNCTIONS
} from './simulator-worker/simulator/evaluator';
import _ from 'lodash';

/**
 * @param a
 * @param b
 * @returns {GCD.props}
 * @constructor
 */
function GCD(a, b) {
  return !b ? a : GCD(b, a % b);
}

/**
 * @param a
 * @param b
 * @returns {number}
 * @constructor
 */
function LCM(a, b) {
  return (a * b) / GCD(a, b);
}

/**
 * @param expr
 * @param inputs
 * @param maxSamples
 * @returns {*}
 */
export function simulate(expr, inputs, maxSamples) {
  return Evaluate(expr, maxSamples, inputs);
}

/**
 * @param text
 * @returns {boolean}
 */
const hasStochasticFunction = text => _.some(STOCHASTIC_FUNCTIONS, e => text.indexOf(e) !== -1);

/**
 * @param text
 * @param inputs
 * @param n
 * @returns {number|*}
 */
export function neededSamples(text, inputs, n) {
  if (hasStochasticFunction(text)) {
    return n;
  }

  if (Object.keys(inputs).length === 0) {
    return 1;
  }

  // TODO(matthew): A more permanent solution should be sought.
  const numInputs = _.filter(inputs, i => !!i)
    .map(i => i.length);
  if (_.some(numInputs, i => i === n)) {
    // No need to compute any further if any of the inputs are maximally sampled. This is a common case so is worth an
    // edge case short circuit here, to avoid gcd/lcm calculation.
    return n;
  }
  return Math.min(n, numInputs.reduce((x, y) => LCM(x, y)));
}

/**
 * @param array
 * @param newStart
 * @returns {*[]}
 */
function rotate(array, newStart) {
  return [...array.slice(newStart), ...array.slice(0, newStart)];
}

/**
 * @param array
 * @param from
 * @param to
 * @returns {*[]|*}
 */
function modularSlice(array, from, to) {
  const len = array.length;
  if (len <= to - from) {
    return rotate(array, to % len);
  }
  const [newFrom, newTo] = [from % len, to % len];
  if (newTo > newFrom) {
    return array.slice(newFrom, newTo);
  }
  return [...array.slice(newFrom), array.slice(0, to)];
}

/**
 * @param str
 * @param replacementMap
 * @returns {*}
 */
function replaceByMap(str, replacementMap) {
  if (!str || _.isEmpty(str) || _.isEmpty(replacementMap)) {
    return str;
  }
  const regex = or(Object.keys(replacementMap));
  return str.replace(regex, match => replacementMap[match]);
}

/**
 * @param rawExpr
 * @param prevModularIndex
 * @param numSamples
 * @param inputs
 * @returns {{numSamples: *, inputs: {}, expr: *}}
 */
function buildSimulationParams(rawExpr, prevModularIndex, numSamples, inputs) {
  let idMap = {};
  let takenReadableIds = [];
  let slicedInputs = {};

  for (let key of Object.keys(inputs)) {
    if (!inputs[key]) {
      console.warn('empty input key passed to buildSimulationParams:', key);
      continue;
    }
    const readableId = 'sdfsdf';//generateRandomReadableId(takenReadableIds)
    idMap[`\$\{${key}\}`] = readableId;
    takenReadableIds.push(readableId);
    slicedInputs[readableId] = modularSlice(inputs[key], prevModularIndex, prevModularIndex + numSamples);
  }

  const expr = replaceByMap(rawExpr, idMap);
  return { expr, numSamples, inputs: slicedInputs };
}

/**
 * @param worker
 * @param data
 * @returns {Promise<unknown>}
 */
function simulateOnWorker(worker, data) {
  return new Promise(
    (resolve, reject) => {
      worker.push(data, ({ data }) => {
        resolve(JSON.parse(data));
      });
    }
  );
}
