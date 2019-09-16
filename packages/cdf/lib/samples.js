const pdfast = require('pdfast');

const { percentile, sortDescending } = require('./dataAnalysis');
const { Pdf } = require('./pdf');

class Samples {
  /**
   * @param samples
   */
  constructor(samples) {
    this.samples = samples;
    this.sorted = [];
  }

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
   * @param {number} min
   * @param {number} max
   * @param {number} size
   * @param {number} width
   * @return {Pdf}
   */
  toPdf({ min, max, size, width }) {
    const args = { size, width };

    if (!!min) args.min = min;
    if (!!max) args.max = max;

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
   * @param min
   * @param max
   * @param size
   * @param width
   * @private
   */
  _kde({ min, max, size, width }) {
    return pdfast.create(this.samples, { min, max, size, width });
  }

}

module.exports = {
  Samples,
};