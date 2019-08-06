const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('./../producer');
const { MeasurableState } = require('./measurable-state');

class MeasurableStateChanged extends MeasurableState {

  /**
   * @param {Models.Measurable} measurable
   */
  constructor(measurable) {
    super(measurable);
    this.templateName = Producer.TEMPLATE_NAME.MEASURABLE_STATE_IS_CHANGED;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return this.measurable.changed('state');
  }


  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      if (await this._isActual() === false) {
        console.log(this.constructor.name, 'Hook is not actual');
        return true;
      }
    } catch (e) {
      console.error(this.constructor.name, e.message, e);
      return false;
    }

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
      console.log(this.constructor.name, e.message, e);
      return false;
    }
  }
}

module.exports = {
  MeasurableStateChanged,
};
