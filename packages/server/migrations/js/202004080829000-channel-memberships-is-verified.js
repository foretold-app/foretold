module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');
      await queryInterface.addColumn('ChannelMemberships', 'isVerified', {
        type: Sequelize.BOOLEAN,
        allowNull: true,
        defaultValue: false,
      });
      await queryInterface.addIndex('ChannelMemberships', ['isVerified'], {
        name: 'ChannelMemberships_isVerified',
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
      await queryInterface.removeColumn('ChannelMemberships', 'isVerified');
      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error('Migration Down', e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  },
};
