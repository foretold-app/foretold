module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.sequelize.query(`CREATE EXTENSION "uuid-ossp"`);
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
      await queryInterface.sequelize.query(`DROP EXTENSION "uuid-ossp"`);
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};

