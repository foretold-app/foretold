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

  /**
   * @todo: fix interface (params, query, options)
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
