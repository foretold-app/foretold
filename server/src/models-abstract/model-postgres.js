const { Model } = require('./model');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class ModelPostgres extends Model {

  constructor(
    {
      model,
      sequelize,
    }
  ) {
    super();
    this.model = model;
    this.sequelize = sequelize;
  }
}

module.exports = {
  ModelPostgres,
};
