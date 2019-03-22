module.exports = {
  up: async function (queryInterface) {
    await queryInterface.removeColumn("Series", "channel");
    await queryInterface.removeColumn("Measurables", "channel");
  },

  down: async function (queryInterface, Sequelize) {
    await queryInterface.addColumn("Series", "channel", {
      type: Sequelize.UUID,
      allowNull: false
    });
    await queryInterface.addColumn("Measurables", "channel", {
      type: Sequelize.UUID,
      allowNull: false
    });
  }
};
