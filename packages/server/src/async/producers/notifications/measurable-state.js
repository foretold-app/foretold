const assert = require('assert');
const _ = require('lodash');

const { getMeasurableLinkWithToken } = require('../../../lib/urls');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../../models/enums/measurement-competitor-type');

const { Producer } = require('../producer');
const { ProducerNotifications } = require('./producer-notifications');

class MeasurableState extends ProducerNotifications {

  /**
   * @param {Models.Measurable} measurable
   */
  constructor(measurable) {
    super({});
    assert(!!_.get(measurable, 'id'), 'Measurable ID is required');
    this.measurable = measurable;
  }

  /**
   * @param {Models.Channel} channel
   * @param {Models.Measurable} measurable
   * @return {{measurable: {name: string, link: string}}}
   * @protected
   */
  static _getReplacements(channel, measurable) {
    return {
      measurable: {
        name: _.get(measurable, 'name'),
        link: getMeasurableLinkWithToken(channel, measurable),
      },
    }
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
    return Producer.data.measurements.getOne(params, options);
  }
}

module.exports = {
  MeasurableState,
};
