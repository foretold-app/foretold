const assert = require('assert');
const _ = require('lodash');

const { getMeasurableLinkWithToken } = require('../../lib/urls');

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
      assert(!!_.get(creator, 'id'), 'Creator ID is required.');

      const channel = await this.measurable.getChannel();
      assert(!!_.get(channel, 'id'), 'Channel ID is required.');

      const replacements = this._getReplacements(channel, this.measurable);
      this._queueEmail(creator, replacements);

    } catch (e) {
      console.log(`MeasurableState`, e.message, e);
    }
    return true;
  }

  /**
   * @param {object} channel
   * @param {object} measurable
   * @return {{measurable: {name: *, link: *}}}
   * @protected
   */
  _getReplacements(channel, measurable) {
    return {
      measurable: {
        name: _.get(measurable, 'name'),
        link: getMeasurableLinkWithToken(channel, measurable),
      },
    }
  }
}

module.exports = {
  MeasurableState,
};
