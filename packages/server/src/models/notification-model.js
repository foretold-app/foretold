const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModels.Generic}
 */
class NotificationModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Notification,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  NotificationModel,
};
