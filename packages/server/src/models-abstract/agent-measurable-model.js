const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentMeasurableModel extends ModelPostgres {
  constructor() {
    super({
      model: models.AgentMeasurable,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  AgentMeasurableModel,
};
