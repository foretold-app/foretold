const assert = require('assert');
const _ = require('lodash');

const { getMeasurableLinkWithToken } = require('../../lib/urls');
const { MEASURABLE_STATE } = require('../../models/enums/measurable-state');

const { Producer } = require('./producer');

class MeasurableState extends Producer {
  constructor(measurable = {}) {
    super({});
    this.measurable = measurable;
  }

  /**
   * @param {object} channel
   * @param {object} measurable
   * @return {{measurable: {name: *, link: *}}}
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
   * @return {Promise<void>}
   * @protected
   */
  async _getLastResolvedMeasurement() {
    assert(!!this.measurable.id, 'Measurable ID is required');
    const params = {
      measurableId: this.measurable.id,
      state: MEASURABLE_STATE.JUDGED,
    };
    const options = await this._getOptions();
    return Producer.data.measurables.getOne(params, options);
  }
}

module.exports = {
  MeasurableState,
};
