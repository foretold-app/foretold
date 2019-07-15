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

  /**
   * @protected
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   */
  applyFilter(where = {}, filter = {}) {
    super.applyFilter(where, filter);

    return where;
  }
}

module.exports = {
  NotificationModel,
};
