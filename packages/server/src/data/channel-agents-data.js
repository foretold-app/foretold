const { DataBase } = require('./data-base');
const { ChannelAgentModel } = require('../models');

/**
 * @implements {Layers.DataSource.Generic}
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
