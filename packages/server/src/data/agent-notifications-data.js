const { DataBase } = require('./data-base');

const { AgentNotificationModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentNotificationModel} AgentNotificationsModel
 */
class AgentNotificationsData extends DataBase {

  constructor() {
    super();
    this.AgentNotificationModel = new AgentNotificationModel();
    this.model = this.AgentNotificationModel;
  }

}

module.exports = {
  AgentNotificationsData,
};
