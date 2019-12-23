module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.removeIndex(
        'Notebooks',
        'Notebooks_name_ownerId_unique',
      );

      await queryInterface.addIndex('Notebooks', [
        'name', 'ownerId', 'channelId',
      ], {
        name: 'Notebooks_name_ownerId_unique',
        unique: true,
      });

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

      await queryInterface.removeIndex(
        'Notebooks',
        'Notebooks_name_ownerId_unique',
      );

      await queryInterface.addIndex('Notebooks', [
        'name', 'ownerId',
      ], {
        name: 'Notebooks_name_ownerId_unique',
        unique: true,
      });

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
