module.exports = {
  up: async function (queryInterface, Sequelize) {
    // Columns
    await queryInterface.addColumn('Users', 'nickname', {
      type: Sequelize.STRING(32),
      allowNull: true,
    });
    await queryInterface.addColumn('Users', 'email', {
      type: Sequelize.STRING(64),
      allowNull: true,
    });
    await queryInterface.addColumn('Users', 'picture', {
      type: Sequelize.STRING(128),
      allowNull: true,
    });

    // Indexes
    await queryInterface.addIndex('Users', ['nickname'], {
      name: 'Users_nickname_unique',
      unique: true,
      where: {
        nickname: {
          [Sequelize.Op.ne]: null,
        },
      },
    });
    await queryInterface.addIndex('Users', ['email'], {
      name: 'Users_email_unique',
      unique: true,
      where: {
        email: {
          [Sequelize.Op.ne]: null,
        },
      },
    });
  },

  down: async function (queryInterface) {
    // Columns
    await queryInterface.removeColumn('Users', 'nickname');
    await queryInterface.removeColumn('Users', 'email');
    await queryInterface.removeColumn('Users', 'picture');

    // Indexes
    await queryInterface.removeIndex('Users', 'Users_nickname_unique');
    await queryInterface.removeIndex('Users', 'Users_email_unique');
  }
};
