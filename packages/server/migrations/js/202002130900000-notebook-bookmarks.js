module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.createTable('NotebookBookmarks', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID,
        },
        agentId: {
          type: Sequelize.UUID,
          references: {
            model: 'Agents',
            key: 'id',
          },
          allowNull: false,
        },
        notebookId: {
          type: Sequelize.UUID,
          references: {
            model: 'Channels',
            key: 'id',
          },
          allowNull: false,
        },
        createdAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
          allowNull: false,
        },
        updatedAt: {
          type: Sequelize.DATE,
          defaultValue: Sequelize.NOW,
          allowNull: false,
        },
      });

      await queryInterface.addIndex('NotebookBookmarks', ['notebookId'], {
        name: 'NotebookBookmarks_notebookId',
      });
      await queryInterface.addIndex('NotebookBookmarks', ['agentId'], {
        name: 'NotebookBookmarks_agentId',
      });
      await queryInterface.addIndex('NotebookBookmarks', ['createdAt'], {
        name: 'NotebookBookmarks_createdAt',
      });
      await queryInterface.addIndex('NotebookBookmarks', ['updatedAt'], {
        name: 'NotebookBookmarks_updatedAt',
      });

      // Unique Indexes
      await queryInterface.addIndex('NotebookBookmarks', [
        'agentId',
        'notebookId',
      ], {
        name: 'NotebookBookmarks_agentId_notebookId',
        unique: true,
      });

      await queryInterface.sequelize.query(
        'ALTER TABLE "NotebookBookmarks" '
        + 'ALTER COLUMN "createdAt" SET DATA TYPE timestamp(3) with time zone, '
        + 'ALTER COLUMN "updatedAt" SET DATA TYPE timestamp(3) with time zone;',
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
      await queryInterface.dropTable('NotebookBookmarks');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
