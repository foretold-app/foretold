const assert = require('assert');
const _ = require('lodash');

const { ProducerFeedItems } = require('./producer-feed-items');

const { Producer } = require('../producer');

class NewMeasurement extends ProducerFeedItems {

  /**
   * @param {Models.Measurement} measurement
   */
  constructor(measurement) {
    super({});

    assert(_.isObject(measurement), 'Measurable should be an object.');
    assert(!!_.get(measurement, 'id'), 'Measurable Id is required.');
    assert(!!_.get(measurement, 'agentId'), 'Agent Id is required.');
    assert(!!_.get(measurement, 'channelId'), 'Agent Id is required.');

    this.measurement = measurement;
    this.templateName =
      Producer.TEMPLATE_NAME.NEW_MEASUREMENT_PREDICTION_FEED_ITEM;
  }

  /**
   * @return {Promise<boolean>}
   */
  async _isActual() {
    return false;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      if (await this._isActual() === false) {
        console.log(this.name, 'Hook is not actual');
        return true;
      }
    } catch (e) {
      console.error(this.name, e.message, e);
      return false;
    }

    try {
      const agent = await Producer.data.agents.getOne({
        id: this.measurement.agentId,
      });
      assert(!!_.get(agent, 'id'), 'Agent ID is required.');

      const replacements = await this._getReplacements(agent);
      const channelId = this.measurement.channelId;
      await this._queueFeedItem(replacements, channelId);

      await this._commit();
      return true;
    } catch (e) {
      await this._rollback();
      console.error(this.name, e.message, e);
      return false;
    }
  }

  /**
   * @param {Models.Agent} agent
   * @return {Promise.<{agent: {name: string}}>}
   * @protected
   */
  async _getReplacements(agent) {
    return {
      agent: {
        name: (await _.get(agent, 'name')) || 'Somebody',
      },
    }
  }

}

module.exports = {
  NewMeasurement,
};
