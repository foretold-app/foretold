const _ = require('lodash');

const { DataBase } = require('./data-base');
const { AgentMeasurablesData } = require('./agent-measurables-data');
const { MeasurablesData } = require('./measurables-data');

const { AgentChannelModel } = require('../models');
const { Proceed } = require('./scoring/proceed');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentChannelModel} model
 */
class AgentChannelsData extends DataBase {
  constructor() {
    super();
    this.model = new AgentChannelModel();
    this.measurables = new MeasurablesData();
    this.agentMeasurables = new AgentMeasurablesData();
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.ChannelID} channelId
   * @returns {Promise<number>}
   */
  async primaryPointScore(agentId, channelId) {
    return this._primaryPointScore(agentId, channelId);
  }

  /**
   * Do not make any optimization here, it is early for this.
   * For each optimization we need to do a researching of the performance.
   * @param {Models.AgentID} agentId
   * @param {Models.ChannelID} channelId
   * @returns {Promise<number>}
   */
  async _primaryPointScore(agentId, channelId) {
    const measurables = await this.model.scoringQuery(agentId, channelId);

    const primaryPointScore$ = measurables.map((item) => {
      return new Proceed().primaryPointScore2(item);
    });

    const sum = _.chain(primaryPointScore$)
      .remove(_.isObject)
      .map((r) => r.score)
      .sum()
      .value();

    return sum;
  }

}

module.exports = {
  AgentChannelsData,
};
