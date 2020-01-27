module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      const tablesWithOrder = [
        'Votes', 'Mutexes', 'FeedItems',
        'NotificationStatuses', 'Notifications',
      ];
      for (const table of tablesWithOrder) {
        await queryInterface.sequelize.query(
          `ALTER TABLE "${table}" SET UNLOGGED;`,
        );
      }

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

      const tablesWithOrder = [
        'Votes', 'Mutexes', 'FeedItems',
        'Notifications', 'NotificationStatuses',
      ];
      for (const table of tablesWithOrder) {
        await queryInterface.sequelize.query(
          `ALTER TABLE "${table}" SET LOGGED;`,
        );
      }

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
