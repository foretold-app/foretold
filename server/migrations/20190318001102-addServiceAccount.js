const _ = require('lodash');

module.exports = {
  up: async function (queryInterface) {
    const [users] = await queryInterface.sequelize.query(
      `SELECT "id" FROM "Users" WHERE "name"='Service Account' LIMIT 1`
    );
    if (users.length === 0) {
      const [agents] = await queryInterface.sequelize.query(
        `INSERT INTO "Agents" ("id", "type") ` +
        `VALUES (uuid_generate_v4(), 'USER')` +
        `RETURNING "id"`
      );
      const agentId = _.get(agents, [0, 'id']);
      await queryInterface.sequelize.query(
        `INSERT INTO "Users" ("id", "name", "auth0Id", "agentId") ` +
        `VALUES ` +
        `(uuid_generate_v4(), 'Service Account', uuid_generate_v4(), '${agentId}')`
      );
    }
  },

  down: async function (queryInterface) {
    await queryInterface.sequelize.query(
      `DELETE FROM "Users" WHERE "name"='Service Account' LIMIT 1`
    );
  }
};
