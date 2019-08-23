const { DataBase } = require('./data-base');

const { SeriesModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {SeriesModel} SeriesModel
 */
class SeriesData extends DataBase {

  constructor() {
    super();
    this.SeriesModel = new SeriesModel();
    this.model = this.SeriesModel;
  }

  /**
   * @public
   * @deprecated: use createdOne
   * @param {object} data
   * @return {Promise<*>}
   */
  async createOne(data) {
    return super.createOne(data);
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
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
