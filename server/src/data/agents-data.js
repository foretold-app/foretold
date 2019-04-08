const { DataBase } = require('./data-base');

const { AgentModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class AgentsData extends DataBase {

  constructor() {
    super();
    this.AgentModel = new AgentModel();
  }

  /**
   * @param {string} id
   * @return {Promise<Models.Agent>}
   */
  async getOne(id) {
    return this.models.Agent.findById(id);
  }
}

module.exports = {
  AgentsData,
};
