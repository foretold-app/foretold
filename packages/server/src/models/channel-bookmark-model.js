const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.Generic}
 */
class ChannelBookmarkModel extends ModelPostgres {
  constructor() {
    super({
      model: models.ChannelBookmark,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  ChannelBookmarkModel,
};
