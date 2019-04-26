class CdfCombination {
  constructor(cdfs) {
    this.cdfs = cdfs;
  }

  minBounds() {
    return min(R.map(e => e.xs[0], this.cdfs))
  }

  maxBounds() {
    return max(R.map(e => e.xs[0], this.cdfs))
  }

  xsToAnalyze(sampleCount) {
    return range(this.minBounds(), this.maxBounds(), sampleCount);
  }

  allYsAtXPoint(xPoint) {
    return R.map(r => r.findY(xPoint), this.cdfs);
  }

  // This assumes equal weighting, which is fine for now
  meanOfYsAtXPoint(xPoint) {
    return R.mean(this.allCdfYsAtXPoint);
  }

  combine(sampleCount) {
    const xs = xsToAnalyze(sampleCount);
    const means = R.map(r => meanOfYsAtXPoint(xPoint), xs);
    return new CDF(xs, means);
  }
}

module.exports = {
  CdfCombination,
};