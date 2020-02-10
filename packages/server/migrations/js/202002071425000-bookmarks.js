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
            model: 'Notebooks',
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

      const table = 'Bookmarks';
      const timestampThree = 'timestamp(3) with time zone';
      await queryInterface.sequelize.query(
        `ALTER TABLE "${table}" `
        + `ALTER COLUMN "createdAt" SET DATA TYPE ${timestampThree}, `
        + `ALTER COLUMN "updatedAt" SET DATA TYPE ${timestampThree};`,
      );

      // @todo: To add a notebook-agent partial constrain.
      // @todo: To add a channel-agent partial constrain.
      // @todo: To add a channel-notebook not-null constrain.
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
