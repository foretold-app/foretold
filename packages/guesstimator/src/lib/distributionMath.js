import math from 'mathjs'

/**
 * The left endpoint marks 0.05% on the CDF, the right 0.95%.
 * @param low
 * @param high
 * @returns {string}
 */
export function distributionUpToIntoLognormal(low, high) {
  let logHigh = math.log(high);
  let logLow = math.log(low);

  let mean = (math.mean(logHigh, logLow)).toFixed(3);
  let stdev = ((logHigh - logLow) / (2 * 1.645)).toFixed(3);

  return `lognormal(${mean},${stdev})`
}

/**
 * The left endpoint marks 0.05% on the CDF, the right 0.95%.
 * @param low
 * @param high
 * @returns {string}
 */
export function distributionUpToIntoNormal(low, high) {
  let mean = (math.mean(high, low)).toFixed(3);
  let stdev = ((high - mean) / 1.645).toFixed(3);
  return `normal(${mean},${stdev})`;
}