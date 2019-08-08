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


  /**
   * @protected
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   */
  applyFilter(where = {}, filter = {}) {
    super.applyFilter(where, filter);

    if (_.has(filter, 'sentAt')) {
      where[this.and].push({
        sentAt: filter.sentAt,
      });
    }

    if (_.has(filter, 'notificationId')) {
      where[this.and].push({
        sentAt: filter.notificationId,
      });
    }

    if (_.has(filter, 'attemptCounterMax')) {
      where[this.and].push({
        attemptCounter: {
          [this.lte]: filter.attemptCounterMax,
        },
      });
    }

    return where;
  }
}

module.exports = {
  AgentNotificationModel,
};
