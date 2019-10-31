const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class NotebookModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Notebook,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  NotebookModel,
};
