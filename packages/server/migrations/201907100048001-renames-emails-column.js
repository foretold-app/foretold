module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
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
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
