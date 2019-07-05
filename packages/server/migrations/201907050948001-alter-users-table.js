module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.addColumn('Users', 'nickname', {
      type: Sequelize.STRING(30),
      allowNull: true,
    });
    await queryInterface.addColumn('Users', 'email', {
      type: Sequelize.STRING(64),
      allowNull: true,
    });
    await queryInterface.addColumn('Users', 'picture', {
      type: Sequelize.STRING(128),
      allowNull: true,
    });
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('Users', 'nickname');
    await queryInterface.removeColumn('Users', 'email');
    await queryInterface.removeColumn('Users', 'picture');
  }
};
