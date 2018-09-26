'use strict';

module.exports = {
  up: async function (queryInterface, Sequelize) {
      await queryInterface.addColumn('Users', "auth0Id", {type: Sequelize.STRING, allowNull: true})
  },

  down: async function (queryInterface, Sequelize) {
  }
};
