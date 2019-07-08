module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
    await queryInterface.addColumn("Measurements", "description", {
      type: Sequelize.TEXT,
      allowNull: true,
    });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
    await queryInterface.removeColumn("Measurements", "description");
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
