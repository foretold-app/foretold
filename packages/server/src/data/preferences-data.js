const { DataBase } = require('./data-base');

const { PreferencesModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {PreferencesModel} PreferencesModel
 */
class PreferencesData extends DataBase {

  constructor() {
    super();
    this.PreferencesModel = new PreferencesModel();
    this.model = this.PreferencesModel;
  }

}

module.exports = {
  PreferencesData,
};
