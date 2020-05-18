const { NewMeasurable } = require('./new-measurable');
const { Producer } = require('../producer');
const { MEASURABLE_STATE } = require('../../../enums');

/**
 * @todo: To rename somehow like "Measurable Reached Resolution".
 */
class NewMeasurableReachedResolution extends NewMeasurable {
  /**
   * @param {Defs.Measurable} measurable
   */
  constructor(measurable) {
    super(measurable);
    this.measurable = measurable;
    this.templateName = Producer.TEMPLATE_NAME
      .MEASURABLE_REACHED_RESOLUTION_DATE_FEED_ITEM;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return this.measurable.changed('state')
      && this.measurable.changed('stateUpdatedAt')
      && this.measurable.previous('state') === MEASURABLE_STATE.OPEN
      && this.measurable.get('state') === MEASURABLE_STATE.JUDGEMENT_PENDING;
  }
}

module.exports = {
  NewMeasurableReachedResolution,
};
