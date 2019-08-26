const { DataBase } = require('./data-base');

const { AgentMeasurableModel } = require('../models-abstract');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {AgentMeasurableModel} AgentMeasurablesModel
 */
class AgentMeasurablesData extends DataBase {

  constructor() {
    super();
    this.AgentMeasurableModel = new AgentMeasurableModel();
    this.model = this.AgentMeasurableModel;
  }

  /**
   * @protected
   * @param {Layers.DataSourceLayer.options} [options]
   * @return {Layers.AbstractModelsLayer.restrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }
}

module.exports = {
  AgentMeasurablesData,
};
