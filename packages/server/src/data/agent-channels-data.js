const _ = require('lodash');

const { DataBase } = require('./data-base');
const { AgentMeasurablesData } = require('./agent-measurables-data');
const { MeasurablesData } = require('./measurables-data');
const { Filter } = require('./classes');

const { AgentChannelModel } = require('../models-abstract');

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
   * Do not make any optimization here, it is early for this.
   * For each optimization we need to do a researching of the performance.
   * @param {Models.AgentID} agentId
   * @param {Models.ChannelID} channelId
   * @returns {Promise<number>}
   */
  async primaryPointScore(agentId, channelId) {
    const measurables = await this._getMeasurables(channelId);

    const primaryPointScore$ = measurables.map((measurable) => {
      return this.agentMeasurables.primaryPointScore(agentId, measurable.id);
    });

    const primaryPointScores = await Promise.all(primaryPointScore$);
    const sum = _.chain(primaryPointScores).remove(_.isObject).map((r) => r.score).sum()
      .value();
    return sum;
  }

  /**
   * @param {Models.ChannelID} channelId
   * @returns {Promise<*>}
   */
  async _getMeasurables(channelId) {
    return this.measurables.getAll(new Filter({
      channelId,
    }));
  }
}

module.exports = {
  AgentChannelsData,
};
