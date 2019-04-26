const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class BotModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Bot,
      sequelize: models.sequelize,
    });
  }

  /**
   * @todo: use super methods
   * @param {object} data
   * @return {data}
   */
  createOne(data) {
    return this.model.create(data);
  }

  /**
   * @todo: use super methods
   * @param {object} params
   * @param {object} data
   * @return {data}
   */
  async updateOne(params, data) {
    const bot = await this.model.findOne({
      where: params,
    });
    if (bot) {
      await bot.update(data);
    }
    return bot;
  }

  /**
   * @todo: use super methods
   * @param {object} filter
   * @param {object} [pagination]
   * @param {object} [restrictions]
   * @return {Promise<void>}
   */
  async getAll(filter, pagination = {}, restrictions = {}) {
    const where = {};
    this.applyRestrictions(where, restrictions);
    return await this.model.findAll({
      limit: pagination.limit,
      offset: pagination.offset,
      where,
    });
  }

  /**
   * @todo: use super methods
   * @param {object} params
   * @param {string} params.id
   * @param {object} query
   * @param {object} restrictions
   * @return {Promise<Models.Bot>}
   */
  async getOne(params, query = {}, restrictions = {}) {
    const where = { id: params.id };
    this.applyRestrictions(where, restrictions);
    return await this.model.findOne({
      where,
    });
  }
}

module.exports = {
  BotModel,
};
