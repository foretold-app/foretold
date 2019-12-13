const { DataBase } = require('./data-base');
const { Params, Query, Data, Options } = require('./classes');

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

  /**
   * @public
   * @param {Models.AgentID} agentId
   * @return {Promise<*>}
   */
  async subscribe(agentId) {
    const params = new Params({ agentId });
    const data = new Data({ stopAllEmails: false });
    const options = new Options();
    return this.updateOne(params, data, options);
  }

  /**
   * @public
   * @param {Models.AgentID} agentId
   * @return {Promise<*>}
   */
  async unsubscribe(agentId) {
    const params = new Params({ agentId });
    const data = new Data({ stopAllEmails: true });
    const options = new Options();
    return this.updateOne(params, data, options);
  }
}

module.exports = {
  PreferencesData,
};
