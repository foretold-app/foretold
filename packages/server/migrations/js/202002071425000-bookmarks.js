module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.createTable('Bookmarks', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID,
        },
        channelId: {
          type: Sequelize.UUID,
          references: {
            model: 'Channels',
            key: 'id',
          },
          allowNull: true,
        },
        notebookId: {
          type: Sequelize.UUID,
          references: {
            model: 'NotebookId',
            key: 'id',
          },
          allowNull: true,
        },
        agentId: {
          type: Sequelize.UUID,
          references: {
            model: 'Agents',
            key: 'id',
          },
          allowNull: true,
        },
        createdAt: {
          type: Sequelize.DATE(3),
          defaultValue: Sequelize.NOW,
          allowNull: false,
        },
        updatedAt: {
          type: Sequelize.DATE(3),
          defaultValue: Sequelize.NOW,
          allowNull: false,
        },
      });

      await queryInterface.addIndex('Bookmarks', ['channelId'], {
        name: 'Bookmarks_channelId',
      });
      await queryInterface.addIndex('Bookmarks', ['notebookId'], {
        name: 'Bookmarks_notebookId',
      });
      await queryInterface.addIndex('Bookmarks', ['agentId'], {
        name: 'Bookmarks_agentId',
      });
      await queryInterface.addIndex('Bookmarks', ['createdAt'], {
        name: 'Bookmarks_createdAt',
      });
      await queryInterface.addIndex('Bookmarks', ['updatedAt'], {
        name: 'Bookmarks_updatedAt',
      });

      await queryInterface.sequelize.query(
        `ALTER TABLE "Bookmarks" SET UNLOGGED`,
      );

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
      await queryInterface.dropTable('Bookmarks');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
