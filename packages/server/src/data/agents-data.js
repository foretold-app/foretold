const { DataBase } = require('./data-base');

const { AgentModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentModel} model
 */
class AgentsData extends DataBase {
  constructor() {
    super();
    this.model = new AgentModel();
  }
}

module.exports = {
  AgentsData,
};
