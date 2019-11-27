const _ = require('lodash');

const { MEASUREMENT_VALUE } = require('./enums');

class MeasurementValue {
  constructor(input) {
    this.input = input;
  }

  static factory(input) {
    const { Binary } = require('./binary');
    const { Comment } = require('./comment');
    const { FloatCdf } = require('./float-cdf');
    const { FloatPoint } = require('./float-point');
    const { Null } = require('./null');
    const { Percentage } = require('./percentage');
    const { UnresolvableResolution } = require('./unresolvable-resolution');

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
      throw new Error('You can only submit one type of value.');
    }

    if (arr.length === 0) {
      throw new Error('You must submit one kind of value.');
    }

    return arr[0] !== undefined ? arr[0] : new Null(input);
  }

  validate() {
    return true;
  }
}

module.exports = {
  MeasurementValue,
};
