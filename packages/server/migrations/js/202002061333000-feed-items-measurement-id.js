module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.addColumn('FeedItems', 'measurementId', {
        type: Sequelize.UUID,
        allowNull: true,
        references: {
          model: 'Measurements',
          key: 'id',
        },
      });

      await queryInterface.addIndex('FeedItems', ['measurementId'], {
        name: 'FeedItems_measurementId',
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
      await queryInterface.removeColumn('FeedItems', 'measurementId');
      await queryInterface.removeIndex(
        'FeedItems',
        'FeedItems_measurementId',
      );
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
