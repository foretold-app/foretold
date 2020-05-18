const { MEASUREMENT_COMPETITOR_TYPE } = require('../../../enums');
const { Producer } = require('../producer');
const { NewMeasurement } = require('./new-measurement');

class NewMeasurementPrediction extends NewMeasurement {
  /**
   * @param {Defs.Measurement} measurement
   */
  constructor(measurement) {
    super(measurement);
    this.measurement = measurement;
    this.templateName = Producer.TEMPLATE_NAME
      .NEW_MEASUREMENT_PREDICTION_FEED_ITEM;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    const { competitorType } = this.measurement;
    return competitorType === MEASUREMENT_COMPETITOR_TYPE.COMPETITIVE;
  }
}

module.exports = {
  NewMeasurementPrediction,
};
