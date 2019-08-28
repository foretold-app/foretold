module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.sequelize.query(`
      CREATE VIEW "AgentChannels" AS
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
               AND "Measurements"."competitorType" IN 
                 ('OBJECTIVE', 'COMPETITIVE', 'AGGREGATION')
          ) AS "numberOfPredictions",
          (
             SELECT count(*)
             FROM "Measurements"
               LEFT JOIN "Measurables" ON
               "Measurables"."id" = "Measurements"."measurableId"
             WHERE "Measurables"."channelId" = "ChannelMemberships"."channelId"
               AND "Measurements"."agentId" = "ChannelMemberships"."agentId"
               AND "Measurements"."competitorType" IN 
                 ('OBJECTIVE', 'COMPETITIVE', 'AGGREGATION')
               AND "Measurables"."state" = 'JUDGED'
          ) AS "numberOfQuestionsScored"
          FROM "ChannelMemberships"
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
      await queryInterface.sequelize.query('DROP VIEW "AgentChannels"');
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
