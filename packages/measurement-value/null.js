const { MeasurementValue } = require('./measurement-value');

class Null extends MeasurementValue {
  validate() {
    return false;
  }
}

module.exports = {
  Null,
};
