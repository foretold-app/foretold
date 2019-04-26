const R = require('ramda');

/**
 * @param {number} xMin
 * @param {number} xMax
 * @param {number} yMin
 * @param {number} yMax
 * @param {number} xIntended
 * @return {number}
 */
function interpolate(xMin, xMax, yMin, yMax, xIntended) {
  const minProportion = (xMax - xIntended) / (xMax - xMin);
  const maxProportion = (xIntended - xMin) / (xMax - xMin);
  return (yMin * minProportion) + (yMax * maxProportion);
}

const min = R.reduce(R.min, Infinity);
const max = R.reduce(R.max, -Infinity);

/**
 * @todo: write a test for me
 * This should return an array of n evenly-spaced items
 * between min and max, including min and max.
 * range(1,5,3) = [1, 3, 5];
 * range(1,5,5) = [1, 2, 3, 4, 5];
 * @param {number} min
 * @param {number} max
 * @param {number} n
 * @return {number[]}
 */
function range(min, max, n) {
  if (n <= 0) throw new RangeError('n is zero');
  if (n === Infinity) throw new RangeError('n is Infinity');
  if (n === 0) return [];
  if (n === 1) return [min];
  if (n === 2) return [min, max];
  n -= 1;
  const diff = min - max;
  const interval = Math.abs(diff / n);

  const result = [];

  let item = min;
  do {
    result.push(item);
    item += interval;
  } while (item <= max);

  // corrects results
  if ((n + 1) - result.length === 1) {
    result.push(b);
  }

  return result;
}

module.exports = {
  interpolate,
  min,
  max,
  range,
};