module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.addColumn('Notebooks', 'description', {
        type: Sequelize.STRING(512),
        allowNull: true,
      });
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
      await queryInterface.removeColumn('Notebooks', 'description');
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
