const { DataBase } = require('./data-base');

const { AgentModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentModel} AgentModel
 */
class AgentsData extends DataBase {

  constructor() {
    super();
    this.AgentModel = new AgentModel();
    this.model = this.AgentModel;
  }

}

module.exports = {
  AgentsData,
};
