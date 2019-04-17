module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      // Warning
      await queryInterface.bulkDelete('Bots', {
        userId: { [Sequelize.Op.eq]: null }
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      await queryInterface.sequelize.query(`ROLLBACK`);
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {

  }
};
