const { MEASUREMENT_COMPETITOR_TYPE } = require('../../../enums');
const { Producer } = require('../producer');
const { NewMeasurement } = require('./new-measurement');

class NewMeasurementResolution extends NewMeasurement {
  /**
   * @param {Defs.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    this.templateName = Producer.TEMPLATE_NAME
      .NEW_MEASUREMENT_RESOLUTION_FEED_ITEM;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return this.input.competitorType === MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE;
  }
}

module.exports = {
  NewMeasurementResolution,
};
