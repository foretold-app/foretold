module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.addColumn("Measurables", "descriptionEntity", {
        type: Sequelize.STRING,
        allowNull: true
      });
      await queryInterface.addColumn("Measurables", "descriptionDate", {
        type: Sequelize.DATE,
        allowNull: true,
      });
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.removeColumn("Measurables", "descriptionEntity");
      await queryInterface.removeColumn("Measurables", "descriptionDate");
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};

