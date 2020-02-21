const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModels.Generic}
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
