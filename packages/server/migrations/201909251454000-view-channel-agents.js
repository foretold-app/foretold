module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.sequelize.query(`
        CREATE OR REPLACE VIEW "ChannelAgents" AS
          WITH
           "MembersFromMeasurements" AS (
              SELECT DISTINCT "Measurables"."channelId", "Measurements"."agentId",
               "Measurements"."createdAt", "Measurements"."updatedAt"
              FROM "Measurables", "Measurements"
              WHERE "Measurements"."measurableId" = "Measurables"."id"
                AND "Measurements"."competitorType" IN ('COMPETITIVE', 'OBJECTIVE')),
           "MembersAsMembers" AS (
               SELECT "agentId", "channelId", "createdAt", "updatedAt"
               FROM "ChannelMemberships"
           )
          SELECT
            DISTINCT ON ("channelId", "agentId")
            "channelId", "agentId", "createdAt", "updatedAt"
          FROM (
              SELECT * FROM "MembersFromMeasurements"
              UNION SELECT * FROM "MembersAsMembers"
          ) AS "ChannelAgents2"
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
      await queryInterface.sequelize.query('DROP VIEW "ChannelAgents"');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};
