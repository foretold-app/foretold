const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
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
