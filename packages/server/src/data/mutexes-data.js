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
    let created = found ? null : await this.createOne(data, options);

    if (found && this.minutes(found) > MutexesData.MUTEX_TTL_MIN) {
      const deleted = await this.deleteOne(params, options);
      created = deleted ? await this.createOne(data, options) : null;
    }

    await this.commit(transaction);

    return created;
  }

  minutes(mutex) {
    const updatedAt = moment(mutex.get('updatedAt'));
    const diff = moment()
      .diff(updatedAt);
    const duration = moment.duration(diff);
    return duration.minutes();
  }
}

MutexesData.MUTEX_TTL_MIN = 5;

module.exports = {
  MutexesData,
};
