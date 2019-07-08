module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
    await queryInterface.createTable('AgentsChannels', {
      agentId: {
        type: Sequelize.UUID,
        allowNull: false,
        references: {
          model: 'Agents',
          key: 'id',
        }
      },
      channelId: {
        type: Sequelize.UUID,
        allowNull: false,
        references: {
          model: 'Channels',
          key: 'id',
        }
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
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
    await queryInterface.dropTable('AgentsChannels');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
