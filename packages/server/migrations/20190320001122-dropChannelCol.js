module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.removeColumn("Series", "channel");
      await queryInterface.removeColumn("Measurables", "channel");
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
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
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
