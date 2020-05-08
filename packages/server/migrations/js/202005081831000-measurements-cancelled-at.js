module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.addColumn('Measurements', 'cancelledAt', {
        type: Sequelize.DATE,
        allowNull: true,
      });
      await queryInterface.addIndex('Measurements', ['cancelledAt'], {
        name: 'Measurements_cancelledAt',
      });
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Up', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.removeColumn('Measurements', 'cancelledAt');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
