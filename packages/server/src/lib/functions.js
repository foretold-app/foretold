const { range } = require('@foretold/cdf/lib/functions');

/**
 * @param {*[]} arr
 * @param {number} n
 * @return {*[]}
 */
function splitBy(arr, n) {
  if (!n) return [];
  if (n < 0) return [];
  if (n === Infinity) return [];
  const min = 0;
  const max = arr.length - 1;
  const indexes = range(min, max, n);
  const rounded = indexes.map((ind) => Math.round(ind));
  const result = [];
  for (const index of rounded) {
    if (arr[index]) {
      result.push(arr[index]);
    }
  }
  return result;
}

module.exports = {
  range,
  splitBy,
};
