const { DataBase } = require('./data-base');

const { AgentChannelModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentChannelModel} AgentChannelsModel
 */
class AgentChannelsData extends DataBase {

  constructor() {
    super();
    this.AgentChannelModel = new AgentChannelModel();
    this.model = this.AgentChannelModel;
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  // _getDefaultRestrictions(options = {}) {
  //   return {
  //     ...super._getDefaultRestrictionsForIncludedIntoMeasurables(options),
  //   };
  // }
}

module.exports = {
  AgentChannelsData,
};
