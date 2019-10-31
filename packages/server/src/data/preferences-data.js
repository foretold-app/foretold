const { DataBase } = require('./data-base');

const { PreferenceModel } = require('../models-abstract');
const { Params, Query, Data } = require('./classes');

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
   * @param {Models.ObjectID} agentId
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
