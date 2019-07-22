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
   * @public
   * @return {Promise<boolean>}
   */
  async stateChanged() {
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

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async stateResolved() {
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
}

module.exports = {
  MeasurableState,
};
