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
   * @returns {Promise<Models.Vote>}
   */
  async upvote(agentId, measurementId) {
    return this._changeVote(agentId, measurementId, this._up);
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @returns {Promise<Models.Vote>}
   */
  async downvote(agentId, measurementId) {
    return this._changeVote(agentId, measurementId, this._down);
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @param {Function} change
   * @returns {Promise<Models.Vote>}
   */
  async _changeVote(agentId, measurementId, change) {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurementId, 'Measurement ID is required.');

    let transaction;
    try {
      transaction = await this.getTransaction();
    } catch (e) {
      this.log.trace(e.message, e);
      throw e;
    }

    try {
      const vote = await this._voteLocked(agentId, measurementId, transaction);
      const voteAmount = change(vote);
      if (voteAmount !== vote.voteAmount) {
        await this._update(vote, voteAmount, transaction);
      }
      await this.commit(transaction);
      return this._vote(agentId, measurementId, transaction);
    } catch (e) {
      this.log.trace(e.message, e);
      await this.rollback(transaction);
      throw e;
    }
  }

  /**
   * @param vote
   * @returns {number}
   * @protected
   */
  _up(vote) {
    let voteAmount = vote.voteAmount;
    if (voteAmount < 0) voteAmount = 0;
    if (voteAmount <= 9) voteAmount += 1;
    return voteAmount;
  }

  /**
   * @param vote
   * @returns {number}
   * @protected
   */
  _down(vote) {
    let voteAmount = vote.voteAmount;
    if (voteAmount > 0) voteAmount = 0;
    if (voteAmount >= -9) voteAmount -= 1;
    return voteAmount;
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @param {Layers.Transaction} transaction
   * @returns {Promise<*>}
   * @private
   */
  async _voteLocked(agentId, measurementId, transaction) {
    const params = new Params({ agentId, measurementId });
    const query = new Query();
    const lock = { level: transaction.LOCK.UPDATE };
    const data = new Data({ agentId, measurementId });
    const options = new Options({ transaction, lock, raw: true });
    return this.upsertOne(params, query, data, options);
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @param {Layers.Transaction} transaction
   * @returns {Promise<*>}
   * @private
   */
  async _vote(agentId, measurementId, transaction) {
    const params = new Params({ agentId, measurementId });
    const query = new Query();
    const options = new Options({ transaction, raw: true });
    return this.upsertOne(params, query, options);
  }

  /**
   * @param {Models.Vote} vote
   * @param {number} voteAmount
   * @param {Layers.Transaction} transaction
   * @returns {Promise<*>}
   * @private
   */
  async _update(vote, voteAmount, transaction) {
    const params = new Params({ id: vote.id });
    const data = new Data({ voteAmount });
    const options = new Options({ transaction, raw: true });
    return this.updateOne(params, data, options);
  }
}

module.exports = {
  VotesData,
};
