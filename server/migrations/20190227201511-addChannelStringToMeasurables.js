'use strict';

module.exports = {
  up: async function (queryInterface, Sequelize){
    await queryInterface.addColumn("Measurables", "channel", {
          type: Sequelize.STRING,
          allowNull:true 
    });
  },

  down: async function (queryInterface, Sequelize){
      await queryInterface.removeColumn("Measurables", "channel");
  }
};
