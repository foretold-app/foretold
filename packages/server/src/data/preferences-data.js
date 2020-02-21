const assert = require('assert');

const { DataBase } = require('./data-base');
const { Params, Query, Data, Options } = require('./classes');
const logger = require('../lib/log');

const { PreferenceModel } = require('../models');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {PreferenceModel} model
 */
class PreferencesData extends DataBase {
  constructor() {
    super();
    this.model = new PreferenceModel();
    this.log = logger.module('PreferencesData');
  }

  /**
   * @public
   * @param {Definitions.AgentID} agentId
   * @return {Promise<*>}
   */
  async getOneByAgentId(agentId) {
    assert(!!agentId, 'Agent ID is required.');
    const params = new Params({ agentId });
    const query = new Query();
    const data = new Data({ agentId });
    return this.upsertOne(params, query, data);
  }

  /**
   * @public
   * @param {Definitions.AgentID} agentId
   * @return {Promise<boolean>}
   */
  async subscribe(agentId) {
    try {
      assert(!!agentId, 'Agent ID is required.');
      const params = new Params({ agentId });
      const data = new Data({ stopAllEmails: false });
      const options = new Options();
      await this.updateOne(params, data, options);
      return true;
    } catch (e) {
      this.log.error(e);
      return false;
    }
  }

  /**
   * @public
   * @param {Definitions.AgentID} agentId
   * @return {Promise<boolean>}
   */
  async unsubscribe(agentId) {
    try {
      assert(!!agentId, 'Agent ID is required.');
      const params = new Params({ agentId });
      const data = new Data({ stopAllEmails: true });
      const options = new Options();
      await this.updateOne(params, data, options);
      return true;
    } catch (e) {
      this.log.error(e);
      return false;
    }
  }
}

module.exports = {
  PreferencesData,
};
