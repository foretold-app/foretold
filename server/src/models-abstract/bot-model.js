const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

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
   * @param {object} pagination
   * @param {object} restrictions
   * @return {Promise<void>}
   */
  async getAll(filter, pagination, restrictions) {
    const where = {};
    this.applyRestrictions(where, restrictions);
    return await this.model.findAll({
      limit: pagination.limit,
      offset: pagination.offset,
      where,
    });
  }

  /**
   * @param {string} id
   * @param {object} query
   * @param {object} restrictions
   * @return {Promise<Model>}
   */
  async getOne({ id }, query, restrictions) {
    const where = { id };
    this.applyRestrictions(where, restrictions);
    return await this.model.findOne({
      where,
    });
  }
}

module.exports = {
  BotModel,
};
