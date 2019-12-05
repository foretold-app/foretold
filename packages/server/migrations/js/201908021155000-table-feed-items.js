module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.createTable('FeedItems', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        channelId: {
          type: Sequelize.UUID,
          allowNull: false,
          references: {
            model: 'Channels',
            key: 'id',
          }
        },
        body: {
          type: Sequelize.JSON,
          allowNull: false,
        },
        createdAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
        updatedAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
        },
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
      await queryInterface.dropTable('FeedItems');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};
