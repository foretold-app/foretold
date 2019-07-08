module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
    await queryInterface.addColumn("Measurables", "descriptionProperty", {
      type: Sequelize.STRING,
      allowNull: true
    });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
    await queryInterface.removeColumn("Measurables", "descriptionProperty");
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

