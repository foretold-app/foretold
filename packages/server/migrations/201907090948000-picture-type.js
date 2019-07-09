module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.changeColumn('Users', 'picture', {
        type: Sequelize.STRING(255),
        allowNull: true,
      });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.changeColumn('Users', 'picture', {
        type: Sequelize.STRING(128),
        allowNull: true,
      });
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
