const moment = require('moment');

const { DataBase } = require('./data-base');
const { Params, Query, Data, Options } = require('./classes');

const { MutexModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {MutexModel} model
 */
class MutexesData extends DataBase {
  constructor() {
    super();
    this.model = new MutexModel();
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {string} name
   * @returns {Promise<*>}
   */
  async take(agentId, name) {
    const transaction = await this.getTransaction();

    let created = null;

    try {
      const params = new Params({ agentId, name });
      const query = new Query();
      const data = new Data({ agentId, name });
      const options = new Options({ transaction });

      await this.lock(options);

      const found = await this.getOne(params, query, options);

      if (found && this.expired(found)) {
        await this.deleteOne(params, query, options);
        created = await this.createOne(data, options);
      }

      if (!found) {
        created = await this.createOne(data, options);
      }

      await this.commit(transaction);
    } catch (err) {
      await this.rollback(transaction);
      throw err;
    }

    return created;
  }

  /**
   * @param mutex
   * @returns {boolean}
   */
  expired(mutex) {
    return this.age(mutex) > MutexesData.MUTEX_TTL_MIN;
  }

  /**
   * @param mutex
   * @returns {number}
   */
  age(mutex) {
    const updatedAt = moment(mutex.get('createdAt'));
    const diff = moment().diff(updatedAt);
    const duration = moment.duration(diff);
    return duration.minutes();
  }

  /**
   * @param {Models.AgentID} agentId
   * @param {string} mutexId
   * @returns {Promise<*>}
   */
  async free(agentId, mutexId) {
    const transaction = await this.getTransaction();

    try {
      const params = new Params({ agentId, id: mutexId });
      const query = new Query();
      const options = new Options({ transaction });

      await this.lock(options);
      await this.deleteOne(params, query, options);

      await this.commit(transaction);
    } catch (e) {
      await this.rollback(transaction);
      throw e;
    }

    return true;
  }
}

MutexesData.MUTEX_TTL_MIN = 5;

module.exports = {
  MutexesData,
};
