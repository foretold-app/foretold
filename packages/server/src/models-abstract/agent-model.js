const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentModel extends ModelPostgres {

  constructor() {
    super({
      model: models.Agent,
      sequelize: models.sequelize,
    });
  }

  /**
   * @protected
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Models.ObjectID} [filter.userId]
   */
  applyFilter(where = {}, filter = {}) {
    super.applyFilter(where, filter);

    if (filter.excludeChannelId) {
      where[this.and].push({
        id: {
          [this.notIn]: this.agentsIdsLiteral(filter.excludeChannelId),
        },
      });
    }

    if (filter.types) {
      where[this.and].push({
        type: {
          [this.in]: filter.types,
        },
      });
    }

    return where;
  }
}

module.exports = {
  AgentModel,
};
