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
        agentId: {
          type: Sequelize.UUID,
          references: {
            model: 'Agents',
            key: 'id',
          },
          allowNull: false,
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
            model: 'Notebooks',
            key: 'id',
          },
          allowNull: true,
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

      // Unique Indexes
      await queryInterface.addIndex('Bookmarks', ['agentId', 'channelId'], {
        name: 'Bookmarks_agentId_channelId',
        unique: true,
        where: {
          channelId: {
            [Sequelize.Op.ne]: null,
          },
        },
      });
      await queryInterface.addIndex('Bookmarks', ['agentId', 'notebookId'], {
        name: 'Bookmarks_agentId_notebookId',
        unique: true,
        where: {
          channelId: {
            [Sequelize.Op.ne]: null,
          },
        },
      });
      await queryInterface.sequelize.query(
        'ALTER TABLE "Bookmarks" '
        + 'ADD CONSTRAINT "one_is_null" '
        + 'CHECK ( '
        + '("notebookId" IS NOT NULL AND "channelId" IS NULL) '
        + ' OR '
        + '("notebookId" IS NULL AND "channelId" IS NOT NULL) '
        + ')',
      );

      await queryInterface.sequelize.query(
        'ALTER TABLE "Bookmarks" '
        + 'ALTER COLUMN "createdAt" SET DATA TYPE timestamp(3) with time zone, '
        + 'ALTER COLUMN "updatedAt" SET DATA TYPE timestamp(3) with time zone;',
      );

      // @todo: To add an authorizer.

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
