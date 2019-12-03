module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.renameColumn(
        'Measurables',
        'descriptionEntity',
        'labelSubject',
      );
      await queryInterface.renameColumn(
        'Measurables',
        'descriptionProperty',
        'labelProperty',
      );
      await queryInterface.renameColumn(
        'Measurables',
        'descriptionDate',
        'labelOnDate',
      );
      await queryInterface.renameColumn(
        'Measurables',
        'description',
        'labelCustom',
      );
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
      await queryInterface.renameColumn(
        'Measurables',
        'labelSubject',
        'descriptionEntity',
      );
      await queryInterface.renameColumn(
        'Measurables',
        'labelProperty',
        'descriptionProperty',
      );
      await queryInterface.renameColumn(
        'Measurables',
        'labelOnDate',
        'descriptionDate',
      );
      await queryInterface.renameColumn(
        'Measurables',
        'labelCustom',
        'description',
      );
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};
