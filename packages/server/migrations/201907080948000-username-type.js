module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.changeColumn('Users', 'name', {
        type: Sequelize.STRING(30),
        allowNull: false,
      });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.changeColumn('Users', 'name', {
        type: Sequelize.STRING(255),
        allowNull: false,
      });
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
