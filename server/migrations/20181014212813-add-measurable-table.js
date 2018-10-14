'use strict';

module.exports = {
  up: async function (queryInterface, Sequelize){
    await queryInterface.addColumn("Measurables", "measurableTableId", {
          type: Sequelize.STRING,
          allowNull:true 
    });
    await queryInterface.addColumn("Measurables", "measurableTableAttributes", {
          type: Sequelize.JSON,
          allowNull: true 
    })
  },

  down: async function (queryInterface, Sequelize){
      await queryInterface.removeColumn("Measurables", "measurableTableId");
      await queryInterface.removeColumn("Measurables", "measurableTableAttributes");
  }
};
