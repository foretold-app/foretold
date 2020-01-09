const tables = [
  'ChannelMemberships', 'Channels', 'FeedItems',
  'GlobalSettings', 'Invitations', 'Mutexes',
  'Notebooks', 'Notifications', 'NotificationStatuses',
  'Preferences', 'Series', 'Templates',
  'Tokens',
];
const columns = ['createdAt', 'updatedAt'];

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      // Not Null
      for (const table of tables) {
        for (const column of columns) {
          await queryInterface.sequelize.query(
            `UPDATE "${table}" ` +
            `SET "${column}" = NOW() ` +
            `WHERE "${column}" IS NULL`,
          );
          await queryInterface.sequelize.query(
            `ALTER TABLE "${table}" ALTER COLUMN "${column}" SET NOT NULL`,
          );
        }
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

      const tables = ['Mutexes'];

      // Nullable
      for (const table of tables) {
        for (const column of columns) {
          await queryInterface.sequelize.query(
            `ALTER TABLE "${table}" ALTER COLUMN "${column}" DROP NOT NULL`,
          );
        }
      }

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
