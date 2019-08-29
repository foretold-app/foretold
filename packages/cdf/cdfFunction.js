const _ = require('lodash');
const { ContinuousDistributionCombination } = require('./continuousDistributionCombination');
const { Cdf } = require('./cdf');
const { Pdf } = require('./pdf');
const { sum } = require("./functions")


// Returns the cutoff value of the specified percentile,
// considering the samples array to be 'length' long.
// samples must be sorted.
/**
 *
 * @param cdfs
 * @param fn
 * @return {cdf}
 */
function fnToCdf(cdfs, fn, sampleCount) {
    let pdfs = cdfs.map(c => c.toPdf());
    let combination = new ContinuousDistributionCombination(pdfs);
    let pdf = combination.combineYsWithFn(sampleCount, fn);
    return pdf;
}

function integrate(cdf, sampleCount){
    sum(cdf.sample(sampleCount));
}

module.exports = {
  fnToCdf,
  integrate
};
