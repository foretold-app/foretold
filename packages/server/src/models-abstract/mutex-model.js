const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class MutexModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Mutex,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  MutexModel,
};
