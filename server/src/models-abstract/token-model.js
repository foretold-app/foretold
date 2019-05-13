const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class TokenModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Token,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  TokenModel,
};
