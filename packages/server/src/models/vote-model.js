const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class VoteModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Vote,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  VoteModel,
};
