const { DataBase } = require('./data-base');

const { BotModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {BotModel} BotModel
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
   * @param {object} params
   * @param {object} data
   * @return {Promise<*>}
   */
  async updateOne(params, data) {
    return await this.BotModel.updateOne(params, data);
  }

  /**
   * @param {object} options
   * @return {Promise<*|Array<Model>>}
   */
  async getAll(options) {
    return this.BotModel.getAll(options);
  }

  /**
   * @param {object} params
   * @param {object} query
   * @param {object} restrictions
   * @return {Promise<void>}
   */
  async getOne(params, query, restrictions) {
    return this.BotModel.getOne(params, query, restrictions);
  }
}

module.exports = {
  BotsData,
};
