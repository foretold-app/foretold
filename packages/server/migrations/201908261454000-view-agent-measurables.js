module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      /**
       * It is a view but not an ordinary table.
       * There is no huge difference between a table
       * and a view for the application, expect the
       * "primaryPointScore" field (it is constant now).
       * We just want to try future features before doing
       * much work.
       */
      await queryInterface.sequelize.query(`
      CREATE VIEW "AgentMeasurables" AS
      SELECT uuid_generate_v4() AS "id",
             "ChannelMemberships"."agentId",
             "Measurables"."id" AS "measurableId",
             0.0                AS "primaryPointScore",
             "ChannelMemberships"."createdAt",
             "ChannelMemberships"."updatedAt"
      FROM "ChannelMemberships",
           "Measurables"
      WHERE "ChannelMemberships"."channelId" = "Measurables"."channelId"
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
      await queryInterface.sequelize.query('DROP VIEW "AgentMeasurables"');
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
