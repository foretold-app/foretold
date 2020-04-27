const assert = require('assert');
const _ = require('lodash');

const {
  getMeasurableLinkWithToken,
} = require('../../../../config/client-urls');
const { MEASUREMENT_COMPETITOR_TYPE } = require('../../../enums');

const { ProducerNotifications } = require('./producer-notifications');

const { Params } = require('../../../data/classes');

/**
 * @abstract
 */
class MeasurableState extends ProducerNotifications {
  /**
   * @param {Defs.Measurable} measurable
   */
  constructor(measurable) {
    super({});
    assert(!!_.get(measurable, 'id'),
      'Measurable ID is required.');
    assert(!!_.get(measurable, 'changed'),
      'Measurable "changed" is required.');
    assert(!!_.get(measurable, 'get'),
      'Measurable "get" is required.');
    assert(!!_.get(measurable, 'previous'),
      'Measurable "previous" is required.');
    this.measurable = measurable;
  }

  /**
   * @param {Defs.Channel} channel
   * @param {Defs.Measurable} measurable
   * @return {{measurable: {name: string, link: string}}}
   * @protected
   */
  async _getReplacements(channel, measurable) {
    return {
      measurable: {
        name: (await _.get(measurable, 'name', null) || 'Question'),
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
    const params = new Params({
      measurableId: this.measurable.id,
      competitorType: MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
    });
    const options = await this._getOptions();
    return this.measurements.getOne(params, options);
  }
}

module.exports = {
  MeasurableState,
};
