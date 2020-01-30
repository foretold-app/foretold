const _ = require('lodash');
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
 * @property {VoteModel} model
 */
class VotesData extends DataBase {
  constructor() {
    super();
    this.model = new VoteModel();
    this.log = logger.module('VotesData');
  }

  /**
   * @param {Models.MeasurementID} measurementId
   * @returns {Promise<number | null>}
   * @public
   */
  async totalVoteAmount(measurementId) {
    const params = new Params({ measurementId });
    const query = new Query({ sort: 0 });
    const options = new Options({ raw: true, attributes: true, group: true });
    const votes = await this.getOne(params, query, options);
    return _.get(votes, 'totalVoteAmount', null);
  }


  /**
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @param {number} voteAmountInput
   * @returns {Promise<Models.Vote>}
   * @public
   */
  async vote(agentId, measurementId, voteAmountInput) {
    assert(!!agentId, 'Agent ID is required.');
    assert(!!measurementId, 'Measurement ID is required.');
    assert(
      !!Number.isInteger(voteAmountInput),
      'Vote amount is required to be integer.',
    );

    let transaction;
    try {
      transaction = await this.getTransaction();
    } catch (e) {
      this.log.trace(e.message, e);
      throw e;
    }

    try {
      const vote = await this._voteLocked(agentId, measurementId, transaction);

      let voteAmountToUpdate = voteAmountInput;

      if (voteAmountToUpdate < -10) {
        voteAmountToUpdate = -10;
      } else if (voteAmountToUpdate > 10) {
        voteAmountToUpdate = 10;
      }

      if (voteAmountToUpdate !== vote.voteAmount) {
        await this._update(vote, voteAmountToUpdate, transaction);
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
   * @param {Models.AgentID} agentId
   * @param {Models.MeasurementID} measurementId
   * @param {Layers.Transaction} transaction
   * @returns {Promise<*>}
   * @protected
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
   * @protected
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
   * @protected
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
