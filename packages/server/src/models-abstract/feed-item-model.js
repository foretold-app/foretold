const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class FeedItemModel extends ModelPostgres {

  constructor() {
    super({
      model: models.FeedItem,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  FeedItemModel,
};
