module.exports = {
  up: async function (queryInterface) {
    try {
    return queryInterface.sequelize.query(`CREATE EXTENSION "uuid-ossp"`);
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
    return queryInterface.sequelize.query(`DROP EXTENSION "uuid-ossp"`);
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};

