const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class BookmarkModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Bookmark,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  BookmarkModel,
};
