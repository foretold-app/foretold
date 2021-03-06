const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class BotModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Bot,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  BotModel,
};
