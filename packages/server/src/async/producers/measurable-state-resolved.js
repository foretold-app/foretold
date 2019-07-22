const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('./producer');
const { MeasurableState } = require('./measurable-state');

class MeasurableStateResolved extends MeasurableState {

  constructor(options) {
    super(options);
    this.templateName = Producer.TEMPLATE_NAME.MEASURABLE_STATE_IS_RESOLVED;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      const channel = await this.measurable.getChannel();
      assert(!!_.get(channel, 'id'), 'Channel ID is required.');

      const agents = await channel.getAgents();
      assert(_.isArray(agents), 'Channel Members are required.');
      assert(agents !== 0, 'Channel Members list is empty.');

      const replacements = MeasurableState._getReplacements(
        channel,
        this.measurable,
      );
      const notification = this._queueEmail(replacements);

      for (let i = 0, max = agents.length; i < max; i++) {
        const agent = agents[i];
        await this._assignNotification(agent, notification);
      }

    } catch (e) {
      console.log(`stateResolved`, e.message, e);
    }
    return true;
  }
}

module.exports = {
  MeasurableStateResolved,
};
