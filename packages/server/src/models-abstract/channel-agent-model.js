const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class ChannelAgentModel extends ModelPostgres {
  constructor() {
    super({
      model: models.ChannelAgent,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Layers.AbstractModelsLayer.options} options
   * @returns {Promise<*>}
   */
  async updateMaterializedView(options) {
    return this._updateMaterializedView('ChannelAgents', options);
  }
}

module.exports = {
  ChannelAgentModel,
};
