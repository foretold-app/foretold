const { DataBase } = require('./data-base');
const { ChannelAgentModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentChannelModel} model
 */
class ChannelAgentsData extends DataBase {
  constructor() {
    super();
    this.model = new ChannelAgentModel();
  }
}

module.exports = {
  ChannelAgentsData,
};
