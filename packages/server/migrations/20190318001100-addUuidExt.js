module.exports = {
  up: async function (queryInterface) {
    return queryInterface.sequelize.query(`CREATE EXTENSION "uuid-ossp"`);
  },

  down: async function (queryInterface) {
    return queryInterface.sequelize.query(`DROP EXTENSION "uuid-ossp"`);
  }
};

