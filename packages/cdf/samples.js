const { interpolate, range, min, max, random } = require('./functions');
const pdfast = require('pdfast')
const {Pdf} = require ('./cdf')

class Samples {
  /**
   * @param {number[]} xs
   * @param {number[]} ys
   */
  constructor(samples) {
      this.samples = samples;
  }

  toPdf({min, max, size, width}){
    let kde = this._kde({min, max, size, width});
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