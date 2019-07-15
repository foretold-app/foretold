const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('./producer');

/**
 * @todo: Rename into "MeasurableStateProducer".
 */
class MeasurableState extends Producer {
  constructor(measurable = {}) {
    super({});
    this.measurable = measurable;
  }

  /**
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      const creator = await this.measurable.getCreator();
      assert(!!_.get(creator, 'id'), 'Creator ID is required');

      this._queueEmail(creator);

    } catch (e) {
      console.log(`MeasurableState`, e.message, e);
    }
    return true;
  }
}

module.exports = {
  MeasurableState,
};
