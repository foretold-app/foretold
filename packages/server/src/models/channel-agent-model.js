const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModels.Generic}
 */
class ChannelAgentModel extends ModelPostgres {
  constructor() {
    super({
      model: models.ChannelAgent,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Layers.AbstractModels.Options} options
   * @returns {Promise<*>}
   */
  async updateMaterializedView(options) {
    return this._updateMaterializedView('ChannelAgents', options);
  }
}

module.exports = {
  ChannelAgentModel,
};
