module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.addColumn('Measurables', 'aggregatedAt', {
      type: Sequelize.DATE,
      allowNull: true,
    });
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('Measurables', 'aggregatedAt');
  }
};
