module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      /**
       * Is it duplicate of "ChannelMemberships"?
       * @todo: think it over more
       */
      await queryInterface.sequelize.query(`
      CREATE VIEW "AgentChannels" AS
      SELECT uuid_generate_v4() AS "id",
             "ChannelMemberships"."agentId",
             "ChannelMemberships"."channelId",
             0.0 AS "primaryPointScore",
             "ChannelMemberships"."createdAt",
             "ChannelMemberships"."updatedAt"
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
