module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.addColumn('Channels', 'knowledgeGraph', {
        type: Sequelize.TEXT,
        allowNull: true,
      });

      await queryInterface.addIndex('Channels', ['knowledgeGraph'], {
        name: 'Channels_knowledgeGraph',
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
      await queryInterface.removeColumn('Channels', 'knowledgeGraph');
      await queryInterface.removeIndex(
        'Channels',
        'Channels_knowledgeGraph',
      );
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
