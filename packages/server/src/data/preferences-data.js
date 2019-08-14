const { DataBase } = require('./data-base');

const { PreferenceModel } = require('../models-abstract');
const { Params } = require('./classes/params');
const { Query } = require('./classes/query');
const { Data } = require('./classes/data');

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
    const params = new Params({ agentId });
    const query = new Query();
    const data = new Data({ agentId });
    return this.upsertOne(params, query, data);
  }
}

module.exports = {
  PreferencesData,
};
