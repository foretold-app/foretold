const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
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
