const { DataBase } = require('./data-base');

const { BotModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 */
class BotsData extends DataBase {

  constructor() {
    super();
    this.BotModel = new BotModel();
  }

  /**
   * @param {object} data
   * @return {Promise<*>}
   */
  async createOne(data) {
    return await this.BotModel.createOne(data);
  }


  /**
   * @param {object} options
   * @param {string[]} options.states
   * @param {string} options.userId
   * @param {number} options.offset
   * @param {number} options.limit
   * @return {Promise<*|Array<Model>>}
   */
  async getAll(options) {
    const pagination = {
      offset: options.offset,
      limit: options.limit,
    };
    const restrictions = {
      userId: true,
      agentId: options.userId,
    };
    return this.BotModel.getAll(options, pagination, restrictions);
  }
}

module.exports = {
  BotsData,
};
