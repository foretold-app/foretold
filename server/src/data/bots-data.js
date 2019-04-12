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

}

module.exports = {
  BotsData,
};
