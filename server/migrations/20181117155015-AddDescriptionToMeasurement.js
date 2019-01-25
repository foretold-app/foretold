'use strict';

module.exports = {
  up: async function (queryInterface, Sequelize){
    await queryInterface.addColumn("Measurements", "description", {
          type: Sequelize.TEXT,
          allowNull: true,
    });
  },

  down: async function (queryInterface, Sequelize){
      await queryInterface.removeColumn("Measurements", "description");
  }
};
