const { ProducerFeedItems } = require('./producer-feed-items');
const { Producer } = require('../producer');
const { MEASURABLE_STATE } = require('../../../models/enums/measurable-state');

class NewMeasurableReachedResolution extends ProducerFeedItems {
  /**
   * @param {Models.Measurable} measurable
   */
  constructor(measurable) {
    super(measurable);
    this.templateName =
      Producer.TEMPLATE_NAME.MEASURABLE_REACHED_RESOLUTION_DATE_FEED_ITEM;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return this.input.changed('state') &&
      this.input.previous('state') === MEASURABLE_STATE.JUDGEMENT_PENDING &&
      this.input.get('state') === MEASURABLE_STATE.JUDGED &&
      this.input.get('expectedResolutionDate') !== null &&
      this.input.get('expectedResolutionDate') < new Date();
  }

  /**
   * @param {Models.Agent} agent
   * @return {Promise.<{agent: {name: string}}>}
   * @protected
   */
  async _getReplacements(agent) {
    return {
      agent: {
        name: (await _.get(agent, 'name')) || 'Somebody',
      },
      measurable: {
        name: this.input.name || 'Question',
      },
    }
  }
}

module.exports = {
  NewMeasurableReachedResolution,
};
