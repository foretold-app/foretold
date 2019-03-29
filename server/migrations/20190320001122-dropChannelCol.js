module.exports = {
  up: async function (queryInterface) {
    await queryInterface.removeColumn("Series", "channel");
    await queryInterface.removeColumn("Measurables", "channel");
  },

  down: async function (queryInterface, Sequelize) {
    await queryInterface.addColumn("Series", "channel", {
      type: Sequelize.TEXT(),
      allowNull: false,
      defaultValue: 'Default Channel',
    });
    await queryInterface.addColumn("Measurables", "channel", {
      type: Sequelize.TEXT(),
      allowNull: false,
      defaultValue: 'Default Channel',
    });
  }
};
