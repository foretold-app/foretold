const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../../../models/enums/measurement-competitor-type');

const { Producer } = require('../producer');

const { NewMeasurement } = require('./new-measurement');

class NewMeasurementPrediction extends NewMeasurement {

  /**
   * @param {Models.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    this.templateName =
      Producer.TEMPLATE_NAME.NEW_MEASUREMENT_RESOLUTION_NOT_AVAILABLE_FEED_ITEM;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    const competitorType = this.measurement.competitorType;
    return competitorType === MEASUREMENT_COMPETITOR_TYPE.UNRESOLVED;
  }

}

module.exports = {
  NewMeasurementPrediction,
};
