const assert = require('assert');
const { DataBase } = require('./data-base');
const { VoteModel } = require('../models');

const { Params } = require('./classes');
const { Query } = require('./classes');
const { Options } = require('./classes');
const { Data } = require('./classes');

const logger = require('../lib/log');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {MutexModel} model
 */
class VotesData extends DataBase {
  constructor() {
    super();
    this.model = new VoteModel();
    this.log = logger.module('VotesData');
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @returns {boolean}
   */
  async upvote(agentId, measurementId) {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurementId, 'Measurement ID is required.');

    let transaction;
    try {
      transaction = await this.getTransaction();
    } catch (e) {
      this.log.trace('upvote', e.message, e);
      throw e;
    }

    try {
      const vote = await this._getVote(agentId, measurementId, transaction);

      let voteAmount = vote.voteAmount;
      if (voteAmount < 0) voteAmount = 0;
      if (voteAmount <= 9) voteAmount += 1;

      await this._updateVote(vote.id, voteAmount, transaction);
      await this.commit(transaction);
    } catch (e) {
      this.log.trace('upvote', e.message, e);
      await this.rollback(transaction);
    }
    return true;
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @returns {boolean}
   */
  async downvote(agentId, measurementId) {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurementId, 'Measurement ID is required.');

    let transaction;
    try {
      transaction = await this.getTransaction();
    } catch (e) {
      this.log.trace('downvote', e.message, e);
      throw e;
    }

    try {
      const vote = await this._getVote(agentId, measurementId, transaction);

      let voteAmount = vote.voteAmount;
      if (voteAmount > 0) voteAmount = 0;
      if (voteAmount >= -9) voteAmount -= 1;

      await this._updateVote(vote.id, voteAmount, transaction);
      await this.commit(transaction);
    } catch (e) {
      this.log.trace('downvote', e.message, e);
      await this.rollback(transaction);
    }
    return true;
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @param {Layers.Transaction} transaction
   * @returns {Promise<*>}
   * @private
   */
  async _getVote(agentId, measurementId, transaction) {
    const params = new Params({ agentId, measurementId });
    const query = new Query();
    const lock = { level: transaction.LOCK.UPDATE };
    const data = new Data({ agentId, measurementId });
    const options = new Options({ transaction, lock, raw: true });
    return this.upsertOne(params, query, data, options);
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {number} voteAmount
   * @param {Layers.Transaction} transaction
   * @returns {Promise<*>}
   * @private
   */
  async _updateVote(agentId, voteAmount, transaction) {
    const params = new Params({ id: agentId });
    const data = new Data({ voteAmount });
    const options = new Options({ transaction, raw: true });
    return this.updateOne(params, data, options);
  }
}

module.exports = {
  VotesData,
};
