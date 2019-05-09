module.exports = {
  up: async function (queryInterface, Sequelize) {
    await queryInterface.removeColumn('Measurables', 'isJudged');
    await queryInterface.removeColumn('Measurables', 'hasResolutionEndpointResolved');
    await queryInterface.addColumn("Measurables", "isArchived", {
      type: Sequelize.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    });
  },

  down: async function (queryInterface, Sequelize) {
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
  }
};
