const { DataBase } = require('./data-base');

const { NotificationStatusModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {NotificationStatusModel} NotificationStatusModel
 */
class NotificationStatusesData extends DataBase {
  constructor() {
    super();
    this.model = new NotificationStatusModel();
  }
}

module.exports = {
  NotificationStatusesData,
};
