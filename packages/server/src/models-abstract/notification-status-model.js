const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
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
