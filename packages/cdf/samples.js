const { interpolate, range, min, max, random } = require('./functions');
const {percentile, sortDescending} = require('./dataAnalysis.js');
const pdfast = require('pdfast')
const {Pdf} = require ('./cdf')

class Samples {
  /**
   * @param {number[]} xs
   * @param {number[]} ys
   */
  constructor(samples) {
      this.samples = samples;
      this.sorted = [];
  }

  sort(){
    this.sorted = sortDescending(this.samples);
  }

  getPercentile(perc){
    const length = this.sorted.length;
    if (length === 0){
      console.error("Samples Percentile: You must sort samples before finding the percentile.")
    }
    return percentile(this.sorted, length, perc);
  }

  toPdf({min, max, size, width}){
    let args = {size, width};
    if (!!min) { args.min = min };
    if (!!max) { args.max = max };
    let kde = this._kde(args);
    let pdf = new Pdf(kde.map(r => r.x), kde.map(r => r.y));
    return pdf;
  }

  toCdf({min, max, size, width}){
      let pdf = this.toPdf({min, max, size, width});
      let cdf = pdf.toCdf();
      return cdf;
  }

  _kde({min, max, size, width}){
      return pdfast.create(this.samples, {min, max, size, width});
  }

}

module.exports = {
  Samples,
};