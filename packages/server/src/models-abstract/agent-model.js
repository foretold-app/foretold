const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Agent,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  AgentModel,
};
