module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
      await queryInterface.sequelize.query('BEGIN');

      // Columns
      await queryInterface.addColumn('Users', 'email', {
        type: Sequelize.STRING(64),
        allowNull: true,
      });
      await queryInterface.addColumn('Users', 'picture', {
        type: Sequelize.STRING(128),
        allowNull: true,
      });

      // Indexes
      await queryInterface.addIndex('Users', ['email'], {
        name: 'Users_email_unique',
        unique: true,
        where: {
          email: {
            [Sequelize.Op.ne]: null,
          },
        },
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

      // Columns
      await queryInterface.removeColumn('Users', 'email');
      await queryInterface.removeColumn('Users', 'picture');

      // Indexes
      await queryInterface.removeIndex('Users', 'Users_email_unique');

      await queryInterface.sequelize.query('COMMIT');
    } catch (e) {
      console.error(e);
      await queryInterface.sequelize.query('ROLLBACK');
      throw e;
    }
  }
};
