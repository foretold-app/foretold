const {
  Cdf,
  ContinuousDistribution,
  ContinuousDistributionCombination,
  scoringFunctions,
} = require("@foretold/cdf");
const _ = require("lodash");

/**
 *
 * @param xs
 * @param ys
 * @returns {{ys: *, xs: *}}
 */
function cdfToPdf({ xs, ys }) {
  let cdf = new Cdf(xs, ys);
  let pdf = cdf.toPdf();
  return { xs: pdf.xs, ys: pdf.ys };
}

/**
 *
 * @param sampleCount
 * @param vars
 * @returns {{ys: *, xs: *}}
 */
function mean(sampleCount, vars) {
  let cdfs = vars.map(r => new Cdf(r.xs, r.ys));
  let comb = new ContinuousDistributionCombination(cdfs);
  let newCdf = comb.combineYsWithMean(sampleCount);

  return { xs: newCdf.xs, ys: newCdf.ys };
}

/**
 *
 * @param sampleCount
 * @param predictionCdf
 * @param aggregateCdf
 * @param resolutionCdf
 */
function scoreMarketCdfCdf(
  sampleCount,
  predictionCdf,
  aggregateCdf,
  resolutionCdf,
) {
  let toCdf = (r) => (new Cdf(r.xs, r.ys));

  return scoringFunctions.distributionInputDistributionOutput({
    predictionCdf: toCdf(predictionCdf),
    aggregateCdf: toCdf(aggregateCdf),
    resultCdf: toCdf(resolutionCdf),
    sampleCount,
  });
}

/**
 *
 * @param sampleCount
 * @param predictionCdf
 * @param resolutionCdf
 */
function scoreNonMarketCdfCdf(sampleCount, predictionCdf, resolutionCdf) {
  let toCdf = (r) => (new Cdf(r.xs, r.ys));

  return scoringFunctions.distributionInputDistributionOutputMarketless({
    predictionCdf: toCdf(predictionCdf),
    resultCdf: toCdf(resolutionCdf),
    sampleCount,
  });
}

/**
 *
 * @param sampleCount
 * @param cdf
 */
function differentialEntropy(sampleCount, cdf) {
  let toCdf = (r) => (new Cdf(r.xs, r.ys));

  return scoringFunctions.differentialEntropy({
    cdf: toCdf(cdf),
    sampleCount: sampleCount
  });
}

/**
 *
 * @param x
 * @param xs
 * @param ys
 * @returns {number}
 */
function findY(x, { xs, ys }) {
  let cdf = new Cdf(xs, ys);
  return cdf.findY(x);
}

/**
 *
 * @param y
 * @param xs
 * @param ys
 * @returns {number}
 */
function findX(y, { xs, ys }) {
  let cdf = new Cdf(xs, ys);
  return cdf.findX(y);
}

/**
 *
 * @param xs
 * @param ys
 * @returns {number[]}
 */
function integral({ xs, ys }) {
  if (_.includes(ys, NaN)){
    return NaN;
  }
  else if (_.includes(ys, Infinity) && _.includes(ys, -Infinity)){
    return NaN;
  }
  else if (_.includes(ys, Infinity)){
    return Infinity;
  }
  else if (_.includes(ys, -Infinity)){
    return -Infinity;
  }

  let integral = 0;
  for (let i = 1; i < ys.length; i++) {
    let thisY = ys[i];
    let lastY = ys[i - 1];
    let thisX = xs[i];
    let lastX = xs[i - 1];

    if (
      _.isFinite(thisY) && _.isFinite(lastY) &&
      _.isFinite(thisX) && _.isFinite(lastX)
    ) {
      let sectionInterval = ((thisY + lastY) / 2) * (thisX - lastX);
      integral = integral + sectionInterval;
    }

  }
  return integral;
}

module.exports = {
  cdfToPdf,
  findY,
  findX,
  mean,
  scoreMarketCdfCdf,
  scoreNonMarketCdfCdf,
  differentialEntropy,
  integral,
};
