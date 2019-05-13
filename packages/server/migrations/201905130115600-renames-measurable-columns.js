module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.addColumn('Agents', 'isAdmin', {
      type: Sequelize.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    });
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('Agents', 'isAdmin');
  }
};
