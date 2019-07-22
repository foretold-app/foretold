const assert = require('assert');
const _ = require('lodash');

const { MeasurableState } = require('./measurable-state');

class MeasurableStateChanged extends MeasurableState {
  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      const creator = await this.measurable.getCreator();
      assert(!!_.get(creator, 'id'), 'Creator ID is required.');

      const channel = await this.measurable.getChannel();
      assert(!!_.get(channel, 'id'), 'Channel ID is required.');

      const replacements = MeasurableState._getReplacements(
        channel,
        this.measurable,
      );
      const notification = this._queueEmail(replacements);
      this._assignNotification(creator, notification);

    } catch (e) {
      console.log(`stateChanged`, e.message, e);
    }
    return true;
  }
}

module.exports = {
  MeasurableStateChanged,
};
