const { DataBase } = require('./data-base');

const { PreferenceModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {PreferenceModel} PreferencesModel
 */
class PreferencesData extends DataBase {

  constructor() {
    super();
    this.PreferenceModel = new PreferenceModel();
    this.model = this.PreferenceModel;
  }

}

module.exports = {
  PreferencesData,
};
