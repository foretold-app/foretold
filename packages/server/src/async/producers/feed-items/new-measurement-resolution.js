const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../../../enums/measurement-competitor-type');
const { Producer } = require('../producer');
const { NewMeasurement } = require('./new-measurement');

class NewMeasurementResolution extends NewMeasurement {

  /**
   * @param {Models.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    this.templateName =
      Producer.TEMPLATE_NAME.NEW_MEASUREMENT_RESOLUTION_FEED_ITEM;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    const competitorType = this.input.competitorType;
    return competitorType === MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE;
  }

}

module.exports = {
  NewMeasurementResolution,
};
