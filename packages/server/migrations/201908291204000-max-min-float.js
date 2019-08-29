module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.changeColumn('Measurables', 'max', {
        type: Sequelize.FLOAT,
        allowNull: true,
      });

      await queryInterface.changeColumn('Measurables', 'min', {
        type: Sequelize.FLOAT,
        allowNull: true,
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);

      await queryInterface.changeColumn('Measurables', 'max', {
        type: Sequelize.INTEGER,
        allowNull: true,
      });

      await queryInterface.changeColumn('Measurables', 'min', {
        type: Sequelize.INTEGER,
        allowNull: true,
      });

      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
