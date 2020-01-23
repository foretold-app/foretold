const assert = require('assert');
const { DataBase } = require('./data-base');
const { VoteModel } = require('../models');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {MutexModel} model
 */
class VotesData extends DataBase {
  constructor() {
    super();
    this.model = new VoteModel();
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @returns {boolean}
   */
  upvote(agentId, measurementId) {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurementId, 'Measurement ID is required.');
    return true;
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @returns {boolean}
   */
  downvote(agentId, measurementId) {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurementId, 'Measurement ID is required.');
    return true;
  }
}

module.exports = {
  VotesData,
};
