module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.renameColumn(
        'Preferences',
        'emails',
        'stopAllEmails',
      );

      await queryInterface.changeColumn('Preferences', 'stopAllEmails', {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        defaultValue: false,
      });
    } catch (e) {
      console.error('Migration Up Error', e);
      throw e;
    }
  },

  down: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.renameColumn(
        'Preferences',
        'stopAllEmails',
        'emails',
      );

      await queryInterface.changeColumn('Preferences', 'emails', {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        defaultValue: true,
      });
    } catch (e) {
      console.error('Migration Down Error', e);
      throw e;
    }
  }
};
