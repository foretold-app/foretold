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
   * @param {Models.MeasurableID} measurableId
   * @returns {boolean}
   */
  upvote(agentId, measurableId) {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurableId, 'Measurable ID is required.');
    return true;
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurableID} measurableId
   * @returns {boolean}
   */
  downvote(agentId, measurableId) {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurableId, 'Measurable ID is required.');
    return true;
  }
}

module.exports = {
  VotesData,
};
