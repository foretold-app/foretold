module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.createTable('AgentNotifications', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID,
        },
        agentId: {
          type: Sequelize.UUID,
          allowNull: false,
          references: {
            model: 'Agents',
            key: 'id',
          },
        },
        notificationId: {
          type: Sequelize.UUID,
          allowNull: false,
          references: {
            model: 'Notifications',
            key: 'id',
          },
        },
        sentAt: {
          type: Sequelize.DATE,
          allowNull: true,
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
      await queryInterface.dropTable('AgentNotifications');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
