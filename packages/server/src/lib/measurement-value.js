const _ = require('lodash');

const { MEASUREMENT_VALUE } = require('../enums');

class MeasurementValue {
  constructor(input) {
    this.input = input;

    const map = Object.values(MEASUREMENT_VALUE);

    this.value = null;
    this.type = MEASUREMENT_VALUE.none;

    for (const type of map) {
      const value = _.get(input, type);
      if (value !== null && value !== undefined) {
        this.value = value;
        this.type = type;
        break;
      }
    }

    if (this.value === null) {
      throw new Error('You must submit one kind of value.');
    }
  }

  /**
   * @public
   * @returns {null|undefined}
   */
  getValue() {
    return this.value;
  }

  /**
   * @public
   * @returns {string|string}
   */
  getType() {
    return this.type;
  }

  /**
   * @public
   * @returns {*}
   */
  validate() {
    switch (this.type) {
      case MEASUREMENT_VALUE.floatCdf: {
        const xs = _.get(this.value, 'xs');
        const sizeXs = _.size(xs);

        const ys = _.get(this.value, 'ys');
        const sizeYs = _.size(ys);

        if (sizeXs !== sizeYs) {
          throw new Error('Xs and Ys should be the same size.');
        }

        if (sizeXs > MeasurementValue.FLOAT_CDF_MAX_XS) {
          throw new Error(`Xs of length (${sizeXs}) `
            + 'exceeds maximum of length '
            + `${MeasurementValue.FLOAT_CDF_MAX_XS}.`);
        }

        return true;
      }

      case MEASUREMENT_VALUE.none:
        return false;

      default:
        return true;
    }
  }
}

MeasurementValue.FLOAT_CDF_MAX_XS = 1000;

module.exports = {
  MeasurementValue,
};
