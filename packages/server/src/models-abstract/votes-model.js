const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class VotesModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Votes,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  VotesModel,
};
