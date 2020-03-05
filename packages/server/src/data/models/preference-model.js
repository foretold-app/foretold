const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class PreferenceModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Preference,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  PreferenceModel,
};
