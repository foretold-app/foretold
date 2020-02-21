const { DataBase } = require('./data-base');
const { SeriesModel } = require('../models');

/**
 * @implements {Layers.DataSource.Generic}
 * @property {SeriesModel} model
 */
class SeriesData extends DataBase {
  constructor() {
    super();
    this.model = new SeriesModel();
  }

  /**
   * @protected
   * @param {Layers.DataSource.Options} [options]
   * @return {Layers.Models.Restrictions}
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
