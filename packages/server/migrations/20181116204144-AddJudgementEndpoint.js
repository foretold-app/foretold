module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
    await queryInterface.addColumn("Measurables", "resolutionEndpoint", {
      type: Sequelize.TEXT,
      allowNull: true
    });
    await queryInterface.addColumn("Measurables", "hasResolutionEndpointResolved", {
      type: Sequelize.BOOLEAN,
      allowNull: false,
      defaultValue: false,
    });
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
    await queryInterface.removeColumn("Measurables", "resolutionEndpoint");
    await queryInterface.removeColumn("Measurables", "hasResolutionEndpointResolved");
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
