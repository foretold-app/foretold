const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentChannelModel extends ModelPostgres {
  constructor() {
    super({
      model: models.AgentChannel,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  AgentChannelModel,
};
