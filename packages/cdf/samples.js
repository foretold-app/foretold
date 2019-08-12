const pdfast = require('pdfast');

const { percentile, sortDescending } = require('./dataAnalysis.js');
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

  getPercentile(perc) {
    const length = this.sorted.length;
    if (length === 0) {
      console.error("Samples Percentile: You must sort samples before finding the percentile.")
    }
    return percentile(this.sorted, length, perc);
  }

  /**
   * @param min
   * @param max
   * @param size
   * @param width
   * @return {Pdf|*}
   */
  toPdf({ min, max, size, width }) {
    let args = { size, width };

    if (!!min) args.min = min;
    if (!!max) args.max = max;

    let kde = this._kde(args);
    console.log("KDE Samples", this.samples);
    console.log("KDE", kde);
    let pdf = new Pdf(kde.map(r => r.x), kde.map(r => r.y));
    return pdf;
  }

  /**
   * @param min
   * @param max
   * @param size
   * @param width
   * @return {Cdf}
   */
  toCdf({ min, max, size, width }) {
    let pdf = this.toPdf({ min, max, size, width });
    let cdf = pdf.toCdf();
    return cdf;
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