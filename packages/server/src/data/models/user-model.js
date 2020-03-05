const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class UserModel extends ModelPostgres {
  constructor() {
    super({
      model: models.User,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  UserModel,
};
