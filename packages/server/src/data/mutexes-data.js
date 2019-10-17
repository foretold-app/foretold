const moment = require('moment');
const { DataBase } = require('./data-base');

const { MutexModel } = require('../models-abstract');
const { MUTEX_STATUS } = require('../enums/mutex-status');

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
   * Do not use small decomposed methods here.
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
    let mutex = await this.upsertOne(params, query, data, options);

    const updatedAt = moment(mutex.get('updatedAt'));
    const diff = moment().diff(updatedAt);
    const duration = moment.duration(diff);
    const minutes = duration.minutes();

    if (minutes > MutexesData.MUTEX_TTL_MIN) {
      const params = new Params({ id: mutex.get('id') });
      const data = new Data({ status: MUTEX_STATUS.FREE });
      const options = new Options({ transaction });
      mutex = await this.updateOne(params, data, options);
    }

    if (mutex.get('status') === MUTEX_STATUS.FREE) {
      const params = new Params({ id: mutex.get('id') });
      const data = new Data({ status: MUTEX_STATUS.CAPTURED });
      const options = new Options({ transaction });
      await this.updateOne(params, data, options);
    }

    await this.commit(transaction);

    return mutex;
  }

}

MutexesData.MUTEX_TTL_MIN = 5;

module.exports = {
  MutexesData,
};
