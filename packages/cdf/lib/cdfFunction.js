const _ = require('lodash');
const { ContinuousDistributionCombination } = require('./continuousDistributionCombination');

/**
 * Returns the cutoff value of the specified percentile,
 * considering the samples array to be 'length' long.
 * samples must be sorted.
 * @param cdfs
 * @param fn
 * @param sampleCount
 * @returns {Cdf}
 */
function fnToCdf(cdfs, fn, sampleCount) {
  let pdfs = cdfs.map(c => c.toPdf());
  let combination = new ContinuousDistributionCombination(pdfs);
  return combination.combineYsWithFn(sampleCount, fn);
}

module.exports = {
  fnToCdf,
};
