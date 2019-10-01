const { DataBase } = require('./data-base');

const { NotificationModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {NotificationModel} NotificationsModel
 */
class NotificationsData extends DataBase {
  constructor() {
    super();
    this.model = new NotificationModel();
  }
}

module.exports = {
  NotificationsData,
};
