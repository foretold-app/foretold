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
   * @param {object} data
   * @return {data}
   */
  createOne(data) {
    return this.model.create(data);
  }

  /**
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
