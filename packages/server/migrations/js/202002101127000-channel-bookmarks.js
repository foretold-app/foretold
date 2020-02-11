module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      await queryInterface.createTable('ChannelBookmarks', {
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

      await queryInterface.addIndex('ChannelBookmarks', ['channelId'], {
        name: 'ChannelBookmarks_channelId',
      });
      await queryInterface.addIndex('ChannelBookmarks', ['agentId'], {
        name: 'ChannelBookmarks_agentId',
      });
      await queryInterface.addIndex('ChannelBookmarks', ['createdAt'], {
        name: 'ChannelBookmarks_createdAt',
      });
      await queryInterface.addIndex('ChannelBookmarks', ['updatedAt'], {
        name: 'ChannelBookmarks_updatedAt',
      });

      // Unique Indexes
      await queryInterface.addIndex('ChannelBookmarks', [
        'agentId',
        'channelId',
      ], {
        name: 'ChannelBookmarks_agentId_channelId',
        unique: true,
      });

      await queryInterface.sequelize.query(
        'ALTER TABLE "ChannelBookmarks" '
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
      await queryInterface.dropTable('ChannelBookmarks');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
