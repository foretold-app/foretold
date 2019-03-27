const models = require('../models');

class DataBase {

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

  channelIdsLiteral(agentId) {
    return models.sequelize.literal(this.channelIds(agentId));
  }

  measurableIds(agentId) {
    return `(
      WITH channelIds AS (${this.channelIds(agentId)})
      SELECT "Measurables"."id" FROM "Measurables"
      WHERE "Measurables"."channelId" IN (SELECT id FROM channelIds)
    )`;
  }

  measurableIdsLiteral(agentId) {
    return models.sequelize.literal(this.measurableIds(agentId));
  }
}

module.exports = {
  DataBase,
};


