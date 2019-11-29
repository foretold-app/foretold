const _ = require('lodash');

const { MeasurementValue } = require('./measurement-value');

class FloatCdf extends MeasurementValue {
  /**
   * @returns {*}
   */
  xs() {
    return _.get(this.input, 'xs');
  }

  /**
   * @returns {*}
   */
  ys() {
    return _.get(this.input, 'ys');
  }

  /**
   * @returns {*}
   */
  sizeXs() {
    return _.size(this.xs());
  }

  /**
   * @returns {*}
   */
  sizeYs() {
    return _.size(this.ys());
  }

  /**
   * @returns {*}
   */
  validate() {
    if (this.sizeXs() !== this.sizeYs()) {
      throw new Error('Xs and Ys should be the same size.');
    }
    if (this.sizeXs() > FloatCdf.MAX_XS) {
      throw new Error(`Xs of length (${ this.sizeXs() }) ` +
        `exceeds maximum of length ${ FloatCdf.MAX_XS }.`);
    }
    return true;
  }
}

FloatCdf.MAX_XS = 1000;

module.exports = {
  FloatCdf,
};
