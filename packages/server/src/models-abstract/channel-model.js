const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class ChannelModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Channel,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  ChannelModel,
};
