module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.addColumn("Measurables", "resolutionEndpoint", {
        type: Sequelize.TEXT,
        allowNull: true
      });
      await queryInterface.addColumn("Measurables", "hasResolutionEndpointResolved", {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        defaultValue: false,
      });
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.removeColumn("Measurables", "resolutionEndpoint");
      await queryInterface.removeColumn("Measurables", "hasResolutionEndpointResolved");
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
