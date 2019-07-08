module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.addColumn('Measurements', 'valueText', {
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
      await queryInterface.removeColumn('Measurements', 'valueText');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
