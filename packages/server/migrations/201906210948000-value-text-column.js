module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.addColumn('Measurements', 'valueText', {
      type: Sequelize.TEXT,
      allowNull: true,
    });
  },

  down: async function (queryInterface) {
    await queryInterface.removeColumn('Measurements', 'valueText');
  }
};
