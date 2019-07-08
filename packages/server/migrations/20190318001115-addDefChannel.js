const _ = require('lodash');

module.exports = {
  up: async function (queryInterface) {
    try {
      const [users] = await queryInterface.sequelize.query(
          `SELECT "agentId" FROM "Users"  WHERE "name"='Service Account' LIMIT 1`
      );
      const agentId = _.get(users, [0, 'agentId']);

      const [channels] = await queryInterface.sequelize.query(
          `SELECT "id" FROM "Channels" WHERE "name"='unlisted' LIMIT 1`
      );

      if (channels.length === 0) {
        const [createdChannels] = await queryInterface.sequelize.query(
            `INSERT INTO "Channels" ("id", "name", "creatorId", "isArchived", "isPublic") ` +
          `VALUES (uuid_generate_v4(), 'unlisted', '${agentId}', FALSE, TRUE)` +
          `RETURNING "id"`
        );
        const channelId = _.get(createdChannels, [0, 'id']);

        await queryInterface.sequelize.query(
            `INSERT INTO "AgentsChannels" ("agentId", "channelId") ` +
          `VALUES ('${agentId}', '${channelId}')`
        );
      }
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(
          `DELETE FROM "Channels" WHERE "name"='unlisted' LIMIT 1`
      );
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
