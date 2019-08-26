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

}

module.exports = {
  AgentMeasurablesData,
};
