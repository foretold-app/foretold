module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.changeColumn('Users', 'name', {
      type: Sequelize.STRING(30),
      allowNull: false,
    });
  },

  down: async function (queryInterface, Sequelize) {
    await queryInterface.changeColumn('Users', 'name', {
      type: Sequelize.STRING(255),
      allowNull: false,
    });
  }
};
