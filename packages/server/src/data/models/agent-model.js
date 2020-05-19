const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class AgentModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Agent,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param _options
   * @returns {*}
   * @protected
   */
  _getAttributes(_options = {}) {
    return {
      include: [
        [this._nameLiteral(), 'name'],
      ],
    };
  }

  /**
   * @protected
   * @return {Sequelize.literal}
   */
  _nameLiteral() {
    return this.literal(this._name());
  }

  /**
   * @todo: To fix later.
   * @protected
   * @return {string}
   */
  _name() {
    // eslint-disable-next-line max-len
    return `( CASE WHEN (SELECT "name" FROM "Users" WHERE "agentId" = "Agent"."id" LIMIT 1)  <> ''
    THEN (SELECT "name" FROM "Users" WHERE "agentId" = "Agent"."id" LIMIT 1)
    ELSE (SELECT "name" FROM "Bots" WHERE "agentId" = "Agent"."id" LIMIT 1)
    END )`;
  }
}

module.exports = {
  AgentModel,
};
