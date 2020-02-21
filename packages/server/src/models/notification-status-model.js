const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.Generic}
 */
class NotificationStatusModel extends ModelPostgres {
  constructor() {
    super({
      model: models.NotificationStatus,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  NotificationStatusModel,
};
