const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentNotificationModel extends ModelPostgres {
  constructor() {
    super({
      model: models.AgentNotification,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  AgentNotificationModel,
};
