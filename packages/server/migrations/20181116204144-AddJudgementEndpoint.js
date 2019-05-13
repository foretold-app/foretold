module.exports = {
  up: async function (queryInterface, Sequelize){
    await queryInterface.addColumn("Measurables", "resolutionEndpoint", {
          type: Sequelize.TEXT,
          allowNull:true 
    });
    await queryInterface.addColumn("Measurables", "hasResolutionEndpointResolved", {
          type: Sequelize.BOOLEAN,
          allowNull: false,
          defaultValue: false,
    })
  },

  down: async function (queryInterface, Sequelize){
      await queryInterface.removeColumn("Measurables", "resolutionEndpoint");
      await queryInterface.removeColumn("Measurables", "hasResolutionEndpointResolved");
  }
};
