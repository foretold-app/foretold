module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      /**
       * Here we replaced "ChannelMemberships" on "ChannelAgents".
       */
      await queryInterface.sequelize.query(`
      CREATE OR REPLACE VIEW "AgentChannels" AS
        SELECT uuid_generate_v4() AS "id",
          "ChannelAgents"."agentId",
          "ChannelAgents"."channelId",
          0.0 AS "primaryPointScore",
          "ChannelAgents"."createdAt",
          "ChannelAgents"."updatedAt",
          (
             SELECT count(DISTINCT "Measurements"."id")
             FROM "Measurements"
               LEFT JOIN "Measurables" ON
               "Measurables"."id" = "Measurements"."measurableId"
             WHERE "Measurables"."channelId" = "ChannelAgents"."channelId"
               AND "Measurements"."agentId" = "ChannelAgents"."agentId"
               AND "Measurements"."competitorType" IN ('OBJECTIVE', 'COMPETITIVE')
          ) AS "numberOfPredictions",
          (
             SELECT count(DISTINCT "Measurables"."id")
             FROM "Measurables", "Measurements"
             WHERE "Measurables"."id" = "Measurements"."measurableId"
               AND "Measurables"."channelId" = "ChannelAgents"."channelId"
               AND "Measurements"."agentId" = "ChannelAgents"."agentId"
               AND "Measurements"."competitorType" IN ('OBJECTIVE', 'COMPETITIVE')
               AND "Measurables"."state" = 'JUDGED'
          ) AS "numberOfQuestionsScored"
          FROM "ChannelAgents"
      `);

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      /**
       * Here we replaced "ChannelAgents" on "ChannelMemberships".
       */
      await queryInterface.sequelize.query(`
      CREATE OR REPLACE VIEW "AgentChannels" AS
        SELECT uuid_generate_v4() AS "id",
          "ChannelMemberships"."agentId",
          "ChannelMemberships"."channelId",
          0.0 AS "primaryPointScore",
          "ChannelMemberships"."createdAt",
          "ChannelMemberships"."updatedAt",
          (
             SELECT count(*)
             FROM "Measurements"
               LEFT JOIN "Measurables" ON
               "Measurables"."id" = "Measurements"."measurableId"
             WHERE "Measurables"."channelId" = "ChannelMemberships"."channelId"
               AND "Measurements"."agentId" = "ChannelMemberships"."agentId"
               AND "Measurements"."competitorType" IN ('OBJECTIVE', 'COMPETITIVE')
          ) AS "numberOfPredictions",
          (
             SELECT count(*)
             FROM "Measurements"
               LEFT JOIN "Measurables" ON
               "Measurables"."id" = "Measurements"."measurableId"
             WHERE "Measurables"."channelId" = "ChannelMemberships"."channelId"
               AND "Measurements"."agentId" = "ChannelMemberships"."agentId"
               AND "Measurements"."competitorType" IN ('OBJECTIVE', 'COMPETITIVE')
               AND "Measurables"."state" = 'JUDGED'
          ) AS "numberOfQuestionsScored"
          FROM "ChannelMemberships"
      `);

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};
