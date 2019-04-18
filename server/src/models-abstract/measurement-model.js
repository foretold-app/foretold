const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class MeasurementModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Measurement,
      sequelize: models.sequelize,
    });
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
      attributes: {
        include: [this.getTotal()]
      },
      where,
      order: [
        ['createdAt', 'DESC'],
      ],
    });
  }
}

module.exports = {
  MeasurementModel,
};
