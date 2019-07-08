module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
    await queryInterface.addColumn("Measurables", "descriptionEntity", {
      type: Sequelize.STRING,
      allowNull: true
    });
    await queryInterface.addColumn("Measurables", "descriptionDate", {
      type: Sequelize.DATE,
      allowNull: true,
    });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
    await queryInterface.removeColumn("Measurables", "descriptionEntity");
    await queryInterface.removeColumn("Measurables", "descriptionDate");
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

