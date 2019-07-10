module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.createTable('Templates', {
        id: {
          allowNull: false,
          primaryKey: true,
          type: Sequelize.UUID
        },
        name: {
          type: Sequelize.STRING(127),
          allowNull: false,
        },
        envelopeTemplate: {
          type: Sequelize.JSON,
          allowNull: false,
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
      await queryInterface.dropTable('Templates');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
