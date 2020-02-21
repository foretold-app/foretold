const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.Generic}
 */
class SeriesModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Series,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  SeriesModel,
};
