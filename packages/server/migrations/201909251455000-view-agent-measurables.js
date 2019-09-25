module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      /**
       * Here we replaced "ChannelMemberships" on "ChannelAgents".
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
            ) AS "predictionCountTotal"
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

      /**
       * Here we replaced "ChannelMemberships" on "ChannelAgents".
       */
      await queryInterface.sequelize.query(`
        CREATE OR REPLACE VIEW "AgentMeasurables" AS
          SELECT
            DISTINCT ON ("Measurables"."id", "ChannelMemberships"."agentId")
            uuid_generate_v4() AS "id",
            0.0 AS "primaryPointScore",
            "Measurables"."id" AS "measurableId",
            "ChannelMemberships"."agentId",
            "ChannelMemberships"."createdAt",
            "ChannelMemberships"."updatedAt",
            (SELECT count(*) FROM "Measurements"
                WHERE "Measurements"."measurableId"  = "Measurables" ."id"
                AND "Measurements"."agentId" = "ChannelMemberships"."agentId"
            ) AS "predictionCountTotal"
          FROM "ChannelMemberships", "Measurables"
          WHERE "ChannelMemberships"."channelId" = "Measurables"."channelId";
      `);

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
