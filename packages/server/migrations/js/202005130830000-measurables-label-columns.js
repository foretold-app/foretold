module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      // Columns
      await queryInterface.addColumn('Measurables', 'labelStartAtDate', {
        type: Sequelize.DATE,
        allowNull: true,
      });
      await queryInterface.addColumn('Measurables', 'labelEndAtDate', {
        type: Sequelize.DATE,
        allowNull: true,
      });
      await queryInterface.addColumn('Measurables', 'labelConditionals', {
        type: Sequelize.ARRAY(Sequelize.STRING),
        allowNull: true,
      });

      // Indexes
      await queryInterface.addIndex('Measurables', ['labelStartAtDate'], {
        name: 'Measurables_labelStartAtDate',
      });
      await queryInterface.addIndex('Measurables', ['labelEndAtDate'], {
        name: 'Measurables_labelEndAtDate',
      });
      await queryInterface.addIndex('Measurables', ['labelConditionals'], {
        name: 'Measurables_labelConditionals',
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
      await queryInterface.removeColumn('Measurables', 'labelStartAtDate');
      await queryInterface.removeColumn('Measurables', 'labelEndAtDate');
      await queryInterface.removeColumn('Measurables', 'labelConditionals');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
