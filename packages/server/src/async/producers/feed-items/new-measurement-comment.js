const _ = require('lodash');

const { MEASUREMENT_VALUE } = require('../../../models/enums/measurement-value');
const { Producer } = require('../producer');
const { NewMeasurement } = require('./new-measurement');

class NewMeasurementComment extends NewMeasurement {

  /**
   * @param {Models.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    this.templateName =
      Producer.TEMPLATE_NAME.NEW_MEASUREMENT_COMMENT_FEED_ITEM;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return _.has(this.input, ['value', MEASUREMENT_VALUE.comment]);
  }

}

module.exports = {
  NewMeasurementComment,
};
