const { DataBase } = require('./data-base');
const { GlobalSettingModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {FeedItemModel} FeedItemModel
 */
class GlobalSettingsData extends DataBase {

  constructor() {
    super();
    this.GlobalSettingModel = new GlobalSettingModel();
    this.model = this.GlobalSettingModel;
  }

}

module.exports = {
  GlobalSettingsData,
};
