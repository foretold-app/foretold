const assert = require('assert');
const _ = require('lodash');

const {
  getMeasurableLinkWithToken,
} = require('../../../../config/client-urls');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../../enums');

const { ProducerNotifications } = require('./producer-notifications');

/**
 * @abstract
 */
class MeasurableState extends ProducerNotifications {
  /**
   * @param {Models.Measurable} measurable
   */
  constructor(measurable) {
    super({});
    assert(!!_.get(measurable, 'id'),
      'Measurable ID is required');
    assert(!!_.get(measurable, 'changed'),
      'Measurable "changed" is required');
    assert(!!_.get(measurable, 'get'),
      'Measurable "get" is required');
    assert(!!_.get(measurable, 'previous'),
      'Measurable "previous" is required');
    this.measurable = measurable;
  }

  /**
   * @param {Models.Channel} channel
   * @param {Models.Measurable} measurable
   * @return {{measurable: {name: string, link: string}}}
   * @protected
   */
  async _getReplacements(channel, measurable) {
    return {
      measurable: {
        name: (await _.get(measurable, 'name') || 'Question'),
        link: getMeasurableLinkWithToken(channel, measurable),
      },
    };
  }

  /**
   * @todo: Is there more better way to understand how to filter out
   * @todo: the author?
   * @return {Promise<Models.Measurement>}
   * @protected
   */
  async _getLastResolvedMeasurement() {
    const params = {
      measurableId: this.measurable.id,
      competitorType: MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
    };
    const options = await this._getOptions();
    return this.measurements.getOne(params, options);
  }
}

module.exports = {
  MeasurableState,
};
