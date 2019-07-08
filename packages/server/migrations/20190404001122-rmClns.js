module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.removeColumn('Measurables', 'isJudged');
      await queryInterface.removeColumn('Measurables', 'hasResolutionEndpointResolved');
      await queryInterface.addColumn("Measurables", "isArchived", {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        defaultValue: false,
      });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.addColumn("Measurables", "isJudged", {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        defaultValue: false,
      });
      await queryInterface.addColumn("Measurables", "hasResolutionEndpointResolved", {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        defaultValue: false,
      });
      await queryInterface.removeColumn('Measurables', 'isArchived');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
