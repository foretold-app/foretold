const _ = require('lodash');
const { CdfCombination } = require('./cdf-combination');
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
    let combination = new CdfCombination(pdfs);
    let xs = combination.xsToAnalyze(sampleCount);
    let cdfValues = xs.map(x => combination.allYsAtXPoint(x));
    let newYs = cdfValues.map(v => fn(v));
    let pdf =new Pdf(xs, newYs); 
    return pdf;
}

function integrate(cdf, sampleCount){
    sum(cdf.sample(sampleCount));
}

module.exports = {
  fnToCdf,
  integrate
};
