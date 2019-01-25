'use strict';

module.exports = {
  up: async function (queryInterface, Sequelize){
    await queryInterface.addColumn("Measurables", "isArchived", {
          type: Sequelize.BOOLEAN,
          allowNull: false, 
          defaultValue: false,
    });
    await queryInterface.addColumn("Measurables", "archivedAt", {
          type: Sequelize.DATE,
          allowNull: true,
    });
  },

  down: async function (queryInterface, Sequelize){
      await queryInterface.removeColumn("Measurables", "isArchived");
      await queryInterface.removeColumn("Measurables", "archivedAt");
  }
};

