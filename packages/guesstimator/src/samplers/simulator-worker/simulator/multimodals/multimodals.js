import math from 'mathjs';
import {
  Discrete
} from 'discrete-sampling';

/**
 * @param d1
 * @param d2
 * @param w
 * @returns {any}
 */
function bimodal(d1, d2, w = math.matrix([0.5])) {
  // A handy shorthand without the need to add square brackets.
  // Check if weights specified as numbers, and if so, turn them into a matrix.
  if (!isNaN(w)) {
    const array_ws = Array.prototype.slice.call(arguments, 2);
    w = math.matrix(array_ws);
  }
  return multimodal(d1, d2, w);
}

// TODO: This sometimes seems to fail when the array isn't long enough.
// filterLessThan(mm(0.2 to 0.9, normal(0.8, 0.3), normal(1, 0.3), normal(1.6, 0.5), normal(2, 0.5), [4,4,1,1]), 0)

// Input should take the form:
// ( d1, d2, d3, ... math.matrix([w1, w2, w3, ...])  where
// dn are numbers/functions which evaluate to numbers when called,
// and wn are weights.
//
// Weights are optional, and if none are specified, the distribution
// will be sampled from uniformly.
//
// Weights can be specified using:
//  1) probability of being sampled (e.g. [0.2, 0.4, 0.4]), or
//  2) any other positive number (e.g. [2, 4, 4] or [1, 2, 2]).
//
// However, if only some weights are specified,
// it will be assumed notation of kind 1) is used, and otherwise
// an error is thrown.
//
// (The math.matrix is there instead of an array
// as mathjs compiles the expression "[]" that way. Might fix later.)
function multimodal() {
  const args = Array.from(arguments);
  let l = args.length;
  let weights;

  // check whether user submited an array of weights
  if (math.typeof(args[l - 1]) == 'Matrix') {
    // separate distributions and weights into
    // separate arrays (and extract array from mathjs matrix).
    weights = args.pop()._data;

    // Check that all arguments are numbers
    if (args.some(isNaN)) {
      throw new TypeError(
        'Input must be coercible to numbers (except weight array)',
      );
    }

    // add implicit weights, uniform over the remaining probability mass
    let diff = args.length - weights.length;
    let sum_w = weights.reduce((a, b) => a + b, 0);
    let uniform_weights = Array(diff)
      .fill((1 - sum_w) / diff);

    weights = weights.concat(uniform_weights);

    // check for error
    if (weights.some(v => v < 0)) {
      throw new RangeError(
        `Some negative weights after normalisation. ` +
        `Did you maybe use custom weights > 1 and ` +
        `forget to specify all of them?`,
      );
    }
  } else {
    // @todo: there should be some kind of TypeError check her as well,
    // @todo: but want it DRY
    weights = Array(l).fill(1 / l);
  }

  // Sample index and get corresponding distribution
  let probabilities = Discrete(weights);
  return args[probabilities.draw()];
}

export const Multimodals = {
  bimodal,
  bi: bimodal,
  multimodal,
  mm: multimodal,
};