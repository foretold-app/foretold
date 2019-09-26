/**
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
  if (n <= 0) throw new RangeError('n is less then zero');
  if (n === Infinity) throw new RangeError('n is Infinity');
  if (n === 0) return [];
  if (n === 1) return [min];
  if (n === 2) return [min, max];
  if (min === max) return Array(n).fill(min);
  n -= 1;
  const diff = min - max;
  const interval = Math.abs(diff / n);

  const result = [];

  let item = min;
  do {
    result.push(item);
    item += interval;
  } while (item <= max);

  // corrects results because of math errors
  if ((n + 1) - result.length === 1) {
    result.push(max);
  }

  return result;
}

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
