module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      /**
       * This part adds "timeAverageScore" column into the view.
       */
      await queryInterface.sequelize.query(`
        CREATE OR REPLACE VIEW "AgentMeasurables" AS
          SELECT
            DISTINCT ON ("Measurables"."id", "ChannelAgents"."agentId")
            uuid_generate_v4() AS "id",
            0.0 AS "primaryPointScore",
            "Measurables"."id" AS "measurableId",
            "ChannelAgents"."agentId",
            "ChannelAgents"."createdAt",
            "ChannelAgents"."updatedAt",
            (SELECT count(*) FROM "Measurements"
                WHERE "Measurements"."measurableId"  = "Measurables" ."id"
                AND "Measurements"."agentId" = "ChannelAgents"."agentId"
                AND "Measurements"."competitorType" IN ('OBJECTIVE', 'COMPETITIVE')
            ) AS "predictionCountTotal",
            0.0 AS "timeAverageScore"
          FROM "ChannelAgents", "Measurables"
          WHERE "ChannelAgents"."channelId" = "Measurables"."channelId";
      `);

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.sequelize.query(`DROP VIEW "AgentMeasurables"`);

      /**
       * However this part removes "timeAverageScore" column from the view.
       */
      await queryInterface.sequelize.query(`
        CREATE OR REPLACE VIEW "AgentMeasurables" AS
          SELECT
            DISTINCT ON ("Measurables"."id", "ChannelAgents"."agentId")
            uuid_generate_v4() AS "id",
            0.0 AS "primaryPointScore",
            "Measurables"."id" AS "measurableId",
            "ChannelAgents"."agentId",
            "ChannelAgents"."createdAt",
            "ChannelAgents"."updatedAt",
            (SELECT count(*) FROM "Measurements"
                WHERE "Measurements"."measurableId"  = "Measurables" ."id"
                AND "Measurements"."agentId" = "ChannelAgents"."agentId"
                AND "Measurements"."competitorType" IN ('OBJECTIVE', 'COMPETITIVE')
            ) AS "predictionCountTotal"
          FROM "ChannelAgents", "Measurables"
          WHERE "ChannelAgents"."channelId" = "Measurables"."channelId";
      `);

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
