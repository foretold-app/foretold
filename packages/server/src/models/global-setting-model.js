const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class GlobalSettingModel extends ModelPostgres {
  constructor() {
    super({
      model: models.GlobalSetting,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  GlobalSettingModel,
};
