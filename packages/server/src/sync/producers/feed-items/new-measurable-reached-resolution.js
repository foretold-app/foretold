const { NewMeasurable } = require('./new-measurable');
const { Producer } = require('../producer');
const { MEASURABLE_STATE } = require('../../../enums/measurable-state');

class NewMeasurableReachedResolution extends NewMeasurable {
  /**
   * @param {Models.Measurable} measurable
   */
  constructor(measurable) {
    super(measurable);
    this.templateName
      = Producer.TEMPLATE_NAME.MEASURABLE_REACHED_RESOLUTION_DATE_FEED_ITEM;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return this.input.changed('state')
      && this.input.changed('stateUpdatedAt')
      && this.input.previous('state') === MEASURABLE_STATE.OPEN
      && this.input.get('state') === MEASURABLE_STATE.JUDGEMENT_PENDING;
  }
}

module.exports = {
  NewMeasurableReachedResolution,
};
