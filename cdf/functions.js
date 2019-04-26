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
 * TODO: This should return an array of n evenly-spaced items between min and max, including min and max.
 * range(1,5,3) = [1, 3, 5];
 * range(1,5,5) = [1, 2, 3, 4, 5];
 * @param {number} min
 * @param {number} max
 * @param {number} n
 * @return {number[]}
 */
function range(min, max, n) {
  // const increment = (maxBounds - minBounds)/n;
}

module.exports = {
  interpolate,
  min,
  max,
  range,
};