const { DataBase } = require('./data-base');

const { AgentModel } = require('../models');

/**
 * @implements {Layers.DataSource.Generic}
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
