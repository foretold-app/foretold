const _ = require('lodash');
const {
  MEASUREMENT_VALUE,
} = require('../../../enums');

const { Producer } = require('../producer');
const { NewMeasurement } = require('./new-measurement');

class NewMeasurementComment extends NewMeasurement {
  /**
   * @param {Defs.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    this.measurement = measurement;
    this.templateName = Producer.TEMPLATE_NAME
      .NEW_MEASUREMENT_COMMENT_FEED_ITEM;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return !!_.get(
      this.measurement,
      ['value', MEASUREMENT_VALUE.comment],
      null,
    );
  }
}

module.exports = {
  NewMeasurementComment,
};
