const _ = require('lodash');

const { MEASUREMENT_VALUE } = require('../models/enums/measurement-value');

class MeasurementValue {
  constructor(input) {
    this.input = input;
  }

  static factory(input) {
    const map = {
      [MEASUREMENT_VALUE.floatCdf]: FloatCdf,
      [MEASUREMENT_VALUE.floatPoint]: FloatPoint,
      [MEASUREMENT_VALUE.percentage]: Percentage,
      [MEASUREMENT_VALUE.binary]: Binary,
      [MEASUREMENT_VALUE.unresolvableResolution]: UnresolvableResolution,
      [MEASUREMENT_VALUE.comment]: Comment,
    };

    const arr = [];

    for (const type in map) {
      const value = _.get(input, type);
      if (value !== null && value !== undefined) {
        const klass = map[type];
        arr.push(new klass(value));
      }
    }

    if (arr.length > 1) {
      throw new Error('You can only submit one type of  value.');
    }

    if (arr.length === 0) {
      throw new Error('Measurable should be in an Open state.');
    }

    return arr[0] !== undefined ? arr[0] : new Null(input);
  }

  validate() {
    return true;
  }
}

class Null extends MeasurementValue {
  validate() {
    return false;
  }
}

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

class FloatPoint extends MeasurementValue {
}

class Percentage extends MeasurementValue {
}

class Binary extends MeasurementValue {
}

class UnresolvableResolution extends MeasurementValue {
}

class Comment extends MeasurementValue {
}

module.exports = {
  MeasurementValue,
  Null,
  FloatCdf,
  FloatPoint,
  Percentage,
  Binary,
  UnresolvableResolution,
  Comment,
};
