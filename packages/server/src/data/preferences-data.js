const { DataBase } = require('./data-base');

const { PreferenceModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {PreferenceModel} PreferencesModel
 */
class PreferencesData extends DataBase {

  constructor() {
    super();
    this.PreferenceModel = new PreferenceModel();
    this.model = this.PreferenceModel;
  }

  /**
   * @public
   * @param {string} agentId
   * @return {Promise<*>}
   */
  async getOneByAgentId(agentId) {
    const params = { agentId };
    const query = {};
    const data = { agentId };
    return this.upsertOne(params, query, data);
  }
}

module.exports = {
  PreferencesData,
};
