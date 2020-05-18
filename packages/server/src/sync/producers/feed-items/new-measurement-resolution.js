const { MEASUREMENT_COMPETITOR_TYPE } = require('../../../enums');
const { Producer } = require('../producer');
const { NewMeasurement } = require('./new-measurement');

class NewMeasurementResolution extends NewMeasurement {
  /**
   * @param {Defs.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    this.measurement = measurement;
    this.templateName = Producer.TEMPLATE_NAME
      .NEW_MEASUREMENT_RESOLUTION_FEED_ITEM;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    const { OBJECTIVE } = MEASUREMENT_COMPETITOR_TYPE;
    return this.measurement.competitorType === OBJECTIVE;
  }
}

module.exports = {
  NewMeasurementResolution,
};
