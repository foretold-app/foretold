const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class AgentChannelModel extends ModelPostgres {
  constructor() {
    super({
      model: models.AgentChannel,
      sequelize: models.sequelize,
    });
  }

  /**
   * @param {Layers.Models.ModelOptions} options
   * @returns {Promise<*>}
   */
  async updateMaterializedView(options) {
    return this._updateMaterializedView('AgentChannels', options);
  }

  /**
   * @param {Defs.AgentID} agentId
   * @param {Defs.ChannelID} channelId
   * @returns {Promise<*>}
   */
  async scoringQuery(agentId, channelId) {
    return this.sequelize.query(`
      SELECT (SELECT row_to_json("d")
              FROM (
                       SELECT "Measurables"."id", "Measurables"."createdAt"
                   ) as "d") as "measurable",

             (SELECT array_to_json(array_agg(row_to_json("d")))
              FROM (
                       SELECT "value", "relevantAt"
                       FROM "Measurements"
                       WHERE "measurableId" = "Measurables"."id"
                         AND "competitorType" = 'AGGREGATION'
                   ) as "d") as "allAggregations",

             (SELECT array_to_json(array_agg(row_to_json("d")))
              FROM (
                       SELECT "value", "relevantAt"
                       FROM "Measurements"
                       WHERE "measurableId" = "Measurables"."id"
                         AND "competitorType" = 'COMPETITIVE'
                         AND "agentId" = :agentId
                   ) as "d") as "agentPredictions",

             (SELECT row_to_json("d")
              FROM (
                       SELECT "value", "relevantAt"
                       FROM "Measurements"
                       WHERE "measurableId" = "Measurables"."id"
                         AND "competitorType" = 'OBJECTIVE'
                       LIMIT 1
                   ) "d")    as "recentResult"
      FROM "Measurables"
      WHERE "channelId" = :channelId
    `, {
      replacements: { channelId, agentId },
      type: this.sequelize.QueryTypes.SELECT,
    });
  }
}

module.exports = {
  AgentChannelModel,
};
