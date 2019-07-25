const _ = require('lodash');
const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
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
