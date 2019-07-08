module.exports = {
  up: async function (queryInterface, Sequelize) {
    try {
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
    } catch (e) {
      console.error(e);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      // Columns
      await queryInterface.removeColumn('Users', 'email');
      await queryInterface.removeColumn('Users', 'picture');

      // Indexes
      await queryInterface.removeIndex('Users', 'Users_email_unique');
    } catch (e) {
      console.error(e);
      throw e;
    }
  }
};
