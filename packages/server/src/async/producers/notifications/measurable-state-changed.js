const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('./../producer');
const { MeasurableState } = require('./measurable-state');

class MeasurableStateChanged extends MeasurableState {

  constructor(options) {
    super(options);
    this.templateName = Producer.TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED;
  }

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
      const notification = await this._queueEmail(replacements);
      await this._assignAgentToNotification(creator, notification);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      console.log(`stateChanged`, e.message, e);
      return false;
    }
  }
}

module.exports = {
  MeasurableStateChanged,
};