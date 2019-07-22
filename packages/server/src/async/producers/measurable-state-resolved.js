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
      assert(_.every(agents, i => _.get(i, 'id')), 'Agent ID is required');

      const lastMeasurement = await this._getLastResolvedMeasurement();
      assert(!!_.get(lastMeasurement, 'id'), 'Last Measurement ID is required');
      assert(!!_.get(lastMeasurement, 'agentId'), 'Agent ID is required');

      const replacements = MeasurableState._getReplacements(channel, this.measurable);
      const notification = await this._queueEmail(replacements);

      const agentsWithoutAuthor = agents.filter((agent) => {
        return agent.id === lastMeasurement.agentId;
      });

      for (let i = 0, max = agentsWithoutAuthor.length; i < max; i++) {
        const agent = agentsWithoutAuthor[i];
        await this._assignNotification(agent, notification);
      }

    } catch (e) {
      await this._rollback();
      console.log(`stateResolved`, e.message, e);
      return false;
    }
    await this._commit();
    return true;
  }
}

module.exports = {
  MeasurableStateResolved,
};
