const { DataBase } = require('./data-base');
const { Params, Query, Data } = require('./classes');

const { PreferenceModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {PreferenceModel} model
 */
class PreferencesData extends DataBase {
  constructor() {
    super();
    this.model = new PreferenceModel();
  }

  /**
   * @public
   * @param {Models.AgentID} agentId
   * @return {Promise<*>}
   */
  async getOneByAgentId(agentId) {
    const params = new Params({ agentId });
    const query = new Query();
    const data = new Data({ agentId });
    return this.upsertOne(params, query, data);
  }
}

module.exports = {
  PreferencesData,
};
