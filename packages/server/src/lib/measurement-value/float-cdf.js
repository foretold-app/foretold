const { MeasurementValue } = require('./measurement-value');

class FloatCdf extends MeasurementValue {
  xs(v) {
    return v === undefined
      ? _.get(this.input, 'xs')
      : _.set(this.input, 'xs', v);
  }

  ys(v) {
    return v === undefined
      ? _.get(this.input, 'ys')
      : _.set(this.input, 'ys', v);
  }

  sizeXs() {
    return _.size(this.xs());
  }

  sizeYs() {
    return _.size(this.ys());
  }

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
