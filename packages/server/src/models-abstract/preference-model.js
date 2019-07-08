const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class PreferencesModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Agent,
      sequelize: models.sequelize,
    });
  }

}

module.exports = {
  PreferencesModel,
};
