module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.createTable('AgentChannels', {
      AgentId: {
        type: Sequelize.UUID,
        allowNull: false,
        references: {
          model: 'Agents',
          key: 'id',
        }
      },
      ChannelId: {
        type: Sequelize.UUID,
        allowNull: false,
        references: {
          model: 'Channels',
          key: 'id',
        }
      },
      createdAt: {
        type: Sequelize.DATE
      },
      updatedAt: {
        type: Sequelize.DATE
      },
    });
  },

  down: async function (queryInterface) {
    await queryInterface.dropTable('AgentChannels');
  }
};

