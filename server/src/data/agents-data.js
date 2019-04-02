const _ = require('lodash');
const models = require('../models');

const { DataBase } = require('./data-base');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class AgentsData extends DataBase {

  /**
   * @param {string} id
   * @return {Promise<Models.Agent>}
   */
  async getOne(id) {
    return models.Agent.findById(id);
  }
}

module.exports = {
  AgentsData,
};
