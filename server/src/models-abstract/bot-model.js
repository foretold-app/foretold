const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

class BotModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Bot,
      sequelize: models.sequelize,
    });
  }

  createOne(data) {
    return this.model.create(data);
  }

  /**
   * @param filter
   * @param pagination
   * @param restrictions
   * @return {Promise<void>}
   */
  async getAll(filter, pagination, restrictions) {
    const where = {};

    this.applyRestrictions(where, restrictions);

    // Query
    return await this.model.findAll({
      limit: pagination.limit,
      offset: pagination.offset,
      where,
    });
  }
}

module.exports = {
  BotModel,
};
