const assert = require('assert');
const _ = require('lodash');

const { Producer } = require('./../producer');
const { MeasurableState } = require('./measurable-state');
const { MEASURABLE_STATE } = require('../../../enums/measurable-state');
const logger = require('../../../lib/log');

const log = logger.module('notifications/measurable-state-resolved');

class MeasurableStateResolved extends MeasurableState {
  /**
   * @param {Models.Measurable} measurable
   */
  constructor(measurable) {
    super(measurable);
    this.templateName = Producer.TEMPLATE_NAME.MEASURABLE_STATE_IS_RESOLVED;
  }

  /**
   * @protected
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return this.measurable.changed('state')
      && this.measurable.get('state') === MEASURABLE_STATE.JUDGED;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      if (await this._isActual() === false) {
        log.trace(this.constructor.name, 'Hook is not actual');
        return true;
      }
    } catch (e) {
      console.error(this.constructor.name, e.message, e);
      return false;
    }

    try {
      /** @type {Models.Channel} */
      const channel = await this.measurable.getChannel();
      assert(!!_.get(channel, 'id'), 'Channel ID is required.');

      /** @type {Models.Agent[]} */
      const agents = await channel.getAgents();
      assert(_.isArray(agents),
        'Channel Members are required.');
      assert(agents.length !== 0,
        'Channel Members list is empty.');
      assert(_.every(agents, (agent) => _.get(agent, 'id')),
        'Agent ID is required');

      /** @type {Models.Measurement} */
      const lastMeasurement = await this._getLastResolvedMeasurement();
      assert(!!_.get(lastMeasurement, 'agentId'), 'Agent ID is required');

      const replacements = MeasurableState._getReplacements(
        channel, this.measurable,
      );
      const notification = await this._queueEmail(replacements);

      const agentsWithoutAuthor = agents.filter((agent) => {
        return agent.id !== lastMeasurement.agentId;
      });

      for (let i = 0, max = agentsWithoutAuthor.length; i < max; i++) {
        const agent = agentsWithoutAuthor[i];
        await this._assignNotification(agent, notification);
      }

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      log.trace(this.constructor.name, e.message, e);
      return false;
    }
  }
}

module.exports = {
  MeasurableStateResolved,
};
