const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModels.Generic}
 */
class NotebookBookmarkModel extends ModelPostgres {
  constructor() {
    super({
      model: models.NotebookBookmark,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  NotebookBookmarkModel,
};
