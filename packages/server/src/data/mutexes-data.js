const moment = require('moment');
const { DataBase } = require('./data-base');

const { MutexModel } = require('../models-abstract');

const { Params, Query, Data, Options } = require('./classes');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {MutexesModel} MutexesModel
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

    const params = new Params({ agentId, name });
    const query = new Query();
    const data = new Data({ agentId, name });
    const options = new Options({ transaction });

    const found = await this.getOne(params, query, options);
    let created = null;

    if (found && this.expired(found)) {
      await this.deleteOne(params, options);
      created = await this.createOne(data, options);
    }

    if (!found) {
      created = await this.createOne(data, options);
    }

    await this.commit(transaction);
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
    const updatedAt = moment(mutex.get('updatedAt'));
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

    const params = new Params({ agentId, id: mutexId });
    const options = new Options({ transaction });

    await this.deleteOne(params, options);

    await this.commit(transaction);
    return true;
  }
}

MutexesData.MUTEX_TTL_MIN = 5;

module.exports = {
  MutexesData,
};
