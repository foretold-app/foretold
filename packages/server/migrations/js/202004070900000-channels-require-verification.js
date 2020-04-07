module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.addColumn('Channels', 'requireVerification', {
        type: Sequelize.BOOLEAN,
        allowNull: false,
        defaultValue: false,
      });
      await queryInterface.addIndex('Channels', ['requireVerification'], {
        name: 'Channels_requireVerification',
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
      await queryInterface.removeColumn('Channels', 'requireVerification');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
