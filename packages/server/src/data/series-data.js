const { DataBase } = require('./data-base');
const { SeriesModel } = require('../models');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {SeriesModel} model
 */
class SeriesData extends DataBase {
  constructor() {
    super();
    this.model = new SeriesModel();
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.Options} [options]
   * @return {Layers.AbstractModelsLayer.Restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      channelId: true,
      isAdmin: options.isAdmin,
      agentId: options.agentId,
    };
  }
}

module.exports = {
  SeriesData,
};
