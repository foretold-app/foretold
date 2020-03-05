const { DataBase } = require('./data-base');
const { SeriesModel } = require('./models');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {SeriesModel} model
 */
class SeriesData extends DataBase {
  constructor() {
    super();
    this.model = new SeriesModel();
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelRestrictions}
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
