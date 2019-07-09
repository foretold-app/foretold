const _ = require('lodash');

module.exports = {
  up: async function (queryInterface) {
    try {
      // Get bunch of duplicated names of users
      const [bunch] = await queryInterface.sequelize.query(
        `SELECT array_agg("id"), "name", count(*) ` +
        `FROM "Users" ` +
        `WHERE "name" != '' ` +
        `GROUP BY "name" ` +
        `HAVING count(*) > 1`
      );

      // Bunches of duplicated names [A, B, C, ...]
      for (let bunchIndex = 0; bunchIndex < bunch.length; bunchIndex++) {
        const users = _.get(bunch, [bunchIndex, 'array_agg']);

        // Bunches of users [1, 2, 3, ...] with name A (then B, C, ...)
        for (let userIndex = 0; userIndex < users.length; userIndex++) {
          const userId = _.get(users, [userIndex]);

          // Adds integer prefix to each duplicated users' names
          await queryInterface.sequelize.query(
            `UPDATE "Users" ` +
            `SET "name" =  '${userIndex}_' || "name" ` +
            `WHERE "id" = '${userId}'`
          );
        }
      }
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    // No down operation
  }
};
