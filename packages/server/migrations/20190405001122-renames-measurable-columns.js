module.exports = {
  up: async function (queryInterface) {
    try {
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
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
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
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
