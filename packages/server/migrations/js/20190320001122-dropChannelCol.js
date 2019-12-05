module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.removeColumn("Series", "channel");
      await queryInterface.removeColumn("Measurables", "channel");
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.addColumn("Series", "channel", {
        type: Sequelize.TEXT(),
        allowNull: false,
        defaultValue: 'Default Channel',
      });
      await queryInterface.addColumn("Measurables", "channel", {
        type: Sequelize.TEXT(),
        allowNull: false,
        defaultValue: 'Default Channel',
      });
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};
