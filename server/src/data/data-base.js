const models = require('../models');

class DataBase {

  /**
   *
   * @todo: Use ORM opportunities to join tables.
   * @todo: Move this restrictions on "model" layer.
   *
   * @param {string} agentId
   * @return {string}
   */
  channelIds(agentId) {
    return agentId ? `(
      SELECT "Channels"."id" FROM "Channels"
      LEFT OUTER JOIN "AgentsChannels" ON "Channels".id = "AgentsChannels"."channelId"
        AND "AgentsChannels"."agentId" = '${agentId}'
      WHERE "Channels"."isPublic" = TRUE OR "AgentsChannels"."agentId" IS NOT NULL
    )` : `(
      SELECT "Channels"."id" FROM "Channels"
      WHERE "Channels"."isPublic" = TRUE
    )`;
  }

  /**
   * @todo: see this.channelIds()
   * @param {string} agentId
   * @return {Sequelize.literal}
   */
  channelIdsLiteral(agentId) {
    return models.sequelize.literal(this.channelIds(agentId));
  }

  /**
   * @todo: see this.channelIds()
   * @param {string} agentId
   * @return {string}
   */
  measurableIds(agentId) {
    return `(
      WITH channelIds AS (${this.channelIds(agentId)})
      SELECT "Measurables"."id" FROM "Measurables"
      WHERE "Measurables"."channelId" IN (SELECT id FROM channelIds)
    )`;
  }

  /**
   * @todo: see this.channelIds()
   * @param {string} agentId
   * @return {Sequelize.literal}
   */
  measurableIdsLiteral(agentId) {
    return models.sequelize.literal(this.measurableIds(agentId));
  }
}

module.exports = {
  DataBase,
};


