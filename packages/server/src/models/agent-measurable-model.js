const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentMeasurableModel extends ModelPostgres {
  constructor() {
    super({
      model: models.AgentMeasurable,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Layers.AbstractModelsLayer.options} options
   * @returns {Promise<*>}
   */
  async updateMaterializedView(options) {
    return this._updateMaterializedView('AgentMeasurables', options);
  }
}

module.exports = {
  AgentMeasurableModel,
};
