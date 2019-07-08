module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.createTable('Preference', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        agentId: {
          type: Sequelize.UUID,
          allowNull: false,
          references: {
            model: 'Agents',
            key: 'id',
          }
        },
        emails: {
          type: Sequelize.BOOLEAN,
          allowNull: false,
          defaultValue: true,
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
      await queryInterface.dropTable('Preference');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
