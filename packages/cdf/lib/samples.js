const pdfast = require('pdfast');

const { percentile, sortDescending } = require('./dataAnalysis');
const { Pdf } = require('./pdf');
const _ = require("lodash");

class Samples {
  /**
   * @param samples
   */
  constructor(samples) {
    this.samples = samples;
    this.sorted = [];
  }

  /**
   *
   */
  sort() {
    this.sorted = sortDescending(this.samples);
  }

  /**
   * @param {number} perc
   * @return {*}
   */
  getPercentile(perc) {
    const length = this.sorted.length;
    if (length === 0) {
      console.error(
        "Samples Percentile: You must sort " +
        "samples before finding the percentile."
      );
    }
    return percentile(this.sorted, length, perc);
  }

  /**
   * @param max
   * @param min
   * @returns {Samples}
   */
  filter({max, min}) {
    let samples = this.samples;
    if (_.isFinite(min)) samples = _.filter(samples, r => r > min);
    if (_.isFinite(max)) samples = _.filter(samples, r => r < max);
    return new Samples(samples);
  }

  /**
   * @param {number} min
   * @param {number} max
   * @param {number} size
   * @param {number} width
   * @return {Pdf}
   */
  toPdf({ min, max, size, width }) {
    const args = { size, width };

    if (_.isFinite(min)) args.min = min;
    if (_.isFinite(max)) args.max = max;

    const kde = this._kde(args);
    return new Pdf(kde.map(r => r.x), kde.map(r => r.y));
  }

  /**
   * @param min
   * @param max
   * @param size
   * @param width
   * @return {Cdf}
   */
  toCdf({ min, max, size, width }) {
    const pdf = this.toPdf({ min, max, size, width });
    return pdf.toCdf();
  }

  /**
   * We don't pass the min/max to the pdfast method, because it is buggy.
   * Mainly: when the max is very large (>100000), then almost nothing seems
   * to render like expected.
   * Second, if the inputed range starts lower than the min, problems happen,
   * though this isn't shown
   * as these values are filtered out 5 lines above.
   * This does mean that the smoothing will lead to there being some
   * probability mass outside the min-max range.
   *
   * @param min
   * @param max
   * @param size
   * @param width
   * @protected
   */
  _kde({ min, max, size, width }) {
    let samples = this.filter({min, max}).samples;
    return pdfast.create(samples, { size, width });
  }

}

module.exports = {
  Samples,
};