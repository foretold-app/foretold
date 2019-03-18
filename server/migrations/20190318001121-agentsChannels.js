module.exports = {
  up: async function (queryInterface, Sequelize) {
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
    });
  },

  down: async function (queryInterface) {
    await queryInterface.dropTable('AgentsChannels');
  }
};
